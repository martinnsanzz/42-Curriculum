# Local modules
from ..models import DroneMap, Hub, Connection, ZoneType
from ..exceptions import GraphError
from .zone import Zone


ZONE_COST: dict[ZoneType, int] = {
    ZoneType.NORMAL: 1,
    ZoneType.PRIORITY: 1,
    ZoneType.RESTRICTED: 2,
}

class Graph:
    def __init__(self, nb_drones: int, zones: dict[str, Zone],
                 adjacency: dict[str, list[str]],
                 link_caps: dict[frozenset[str], int],
                 start: str, end: str) -> None:
        self._nb_drones = nb_drones
        self._zones = zones
        self._adjacency = adjacency
        self._link_caps = link_caps
        self.start = start
        self.end = end

    @classmethod
    def from_drone_map(cls, drone_map: DroneMap) -> "Graph":
        nb_drones = drone_map.nb_drones
        zones = cls._build_zones(drone_map)
        adjacency = cls._build_adjacency(zones, drone_map.connections)
        link_caps = cls._build_link_caps(drone_map.connections)

        return cls(nb_drones, zones, adjacency, link_caps,
                   drone_map.start.name, drone_map.end.name)

    @classmethod
    def _build_zones(cls, drone_map: DroneMap) -> dict[str, Zone]:
        zones: dict[str, Zone] = {}

        zones[drone_map.start.name] = cls._build_zone(drone_map.start)
        for hub in drone_map.hubs:
            zones[hub.name] = cls._build_zone(hub)
        zones[drone_map.end.name] = cls._build_zone(drone_map.end)
        return zones

    @staticmethod
    def _build_zone(hub: Hub) -> Zone:
        """Convert a validated `Hub` into a `Zone`."""
        return Zone(
            hub.name,
            hub.x,
            hub.y,
            hub.metadata.zone_type,
            hub.metadata.color,
            hub.metadata.max_drones
        )

    @staticmethod
    def _build_adjacency(zones: dict[str, Zone],
                     connections: list[Connection]) -> dict[str, list[str]]:
        adjacency = {name: [] for name in zones}

        for conn in connections:
            if conn.name1 in zones and conn.name2 in zones:
                adjacency[conn.name1].append(conn.name2)
                adjacency[conn.name2].append(conn.name1)
        return adjacency

    @staticmethod
    def _build_link_caps(connections: list[Connection]) \
                            -> dict[frozenset[str], int]:
        link_caps = dict()

        for conn in connections:
            tmp_set = frozenset({conn.name1, conn.name2})
            link_caps.update({tmp_set:conn.metadata.max_link_capacity})
        return link_caps

    def valid_neighbours(self, name: str) -> list[str]:
        return [x for x in self._adjacency[name] if not self.is_blocked(x)]

    def all_neighbours(self, name: str) -> list[str]:
        return list(self._adjacency[name])

    def is_blocked(self, name: str) -> bool:
        if self._zones[name].zone_type == ZoneType.BLOCKED:
            return True
        return False

    def cost(self, name: str) -> int:
        zone = self._zones[name].zone_type

        if zone == ZoneType.BLOCKED:
            raise GraphError("Algorithm tried to enter " \
                             f"blocked zone '{name}'")
        elif zone not in ZONE_COST:
            raise GraphError(f"Algorithm tried to enter '{name}', " \
                         f"a zone of type '{zone.value}'")
        return ZONE_COST[zone]

    def check_hub_priority(self, name:str) -> int:
        if self._zones[name].zone_type == ZoneType.PRIORITY:
            return -1
        return 0

    def zone_capacity(self, name: str) -> int:
        return self._zones[name].max_drones

    def link_capacity(self, a: str, b: str) -> int:
        for name in (a, b):
            if name not in self._zones:
                raise GraphError(f"Unknown zone '{name}'")

        key = frozenset({a, b})
        if key not in self._link_caps:
            raise GraphError(f"No link found between '{a}' and '{b}'")
        return self._link_caps[key]