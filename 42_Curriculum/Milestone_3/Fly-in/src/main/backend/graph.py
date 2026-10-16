# Local modules
from ..models import DroneMap, Hub, Connection, Zone
from ..exceptions import GraphError

ZONE_COST: dict[Zone, int] = {
    Zone.NORMAL: 1,
    Zone.PRIORITY: 1,
    Zone.RESTRICTED: 2,
}

class Graph():
    def __init__(self, hubs: dict[str, Hub],
                 adjacency: dict[str, list[str]],
                 link_caps: dict[frozenset[str], int],
                 start: str, end: str) -> None:
        self._hubs = hubs
        self._adjacency = adjacency
        self._link_caps = link_caps
        self.start = start
        self.end = end

    @classmethod
    def from_drone_map(cls, drone_map: DroneMap) -> "Graph":
        hubs = cls._build_hubs(drone_map)
        adjacency = cls._build_adjacency(hubs, drone_map.connections)
        link_caps = cls._build_link_caps(drone_map.connections)

        return cls(hubs, adjacency, link_caps,
                   drone_map.start.name, drone_map.end.name)

    @staticmethod
    def _build_hubs(drone_map: DroneMap) -> dict[str, Hub]:
        hubs = dict()

        hubs.update({drone_map.start.name: drone_map.start})
        for hub in drone_map.hubs:
            hubs.update({hub.name: hub})
        hubs.update({drone_map.end.name: drone_map.end})
        return hubs

    @staticmethod
    def _build_adjacency(hubs: dict[str, Hub],
                     connections: list[Connection]) -> dict[str, list[str]]:
        adjacency = {name: [] for name in hubs}

        for conn in connections:
            if conn.name1 in hubs and conn.name2 in hubs:
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
        if self._hubs[name].metadata.zone == Zone.BLOCKED:
            return True
        return False

    def cost(self, name: str) -> int:
        zone = self._hubs[name].metadata.zone

        if zone == Zone.BLOCKED:
            raise GraphError("Algorithm tried to enter " \
                             f"blocked zone '{name}'")
        elif zone not in ZONE_COST:
            raise GraphError(f"Algorithm tried to enter '{name}', " \
                         f"a zone of type '{zone.value}'")
        return ZONE_COST[zone]

    def zone_capacity(self, name: str) -> int:
        return self._hubs[name].metadata.max_drones

    def link_capacity(self, a: str, b: str) -> int:
        for name in (a, b):
            if name not in self._hubs:
                raise GraphError(f"Unknown zone '{name}'")

        key = frozenset({a, b})
        if key not in self._link_caps:
            raise GraphError(f"No link found between '{a}' and '{b}'")
        return self._link_caps[key]