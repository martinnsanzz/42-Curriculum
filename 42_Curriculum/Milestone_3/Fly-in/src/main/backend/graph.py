# Local modules
from ..models import DroneMap, Hub, Connection, HubType
from ..exceptions import GraphError


HUB_COST: dict[HubType, int] = {
    HubType.NORMAL: 1,
    HubType.PRIORITY: 1,
    HubType.RESTRICTED: 2,
}


class Graph:
    """Undirected hub graph built from a validated DroneMap.

    Hold the hubs, their adjacency lists and the capacity of every link.
    Provide the lookups used by the pathfinding and simulation code. Build
    instances with `Graph.from_drone_map` rather than calling the
    constructor directly.

    Args:
        nb_drones (int): Total number of drones in the simulation.
        hubs (dict[str, hub]): hubs keyed by name.
        adjacency (dict[str, list[str]]): Neighbour names for every hub
            name.
        link_caps (dict[frozenset[str], int]): Capacity of each link,
            keyed by the unordered pair of hub names.
        start (str): Name of the start hub.
        end (str): Name of the end hub.

    Attributes:
        start (str): Name of the start hub.
        end (str): Name of the end hub.
    """
    def __init__(self, nb_drones: int, hubs: dict[str, Hub],
                 adjacency: dict[str, list[str]],
                 link_caps: dict[frozenset[str], int],
                 start: str, end: str) -> None:
        self._nb_drones = nb_drones
        self._hubs = hubs
        self._adjacency = adjacency
        self._link_caps = link_caps
        self.start = start
        self.end = end

    @classmethod
    def from_drone_map(cls, drone_map: DroneMap) -> "Graph":
        """Build a Graph from a validated DroneMap.

        Args:
            drone_map (DroneMap): The validated map to convert.

        Returns:
            Graph: A graph with one hub per hub (including start and
                end), adjacency lists, and link capacities.
        """
        nb_drones = drone_map.nb_drones
        hubs = cls._build_hubs(drone_map)
        adjacency = cls._build_adjacency(hubs, drone_map.connections)
        link_caps = cls._build_link_caps(drone_map.connections)

        return cls(nb_drones, hubs, adjacency, link_caps,
                   drone_map.start.name, drone_map.end.name)

    @classmethod
    def _build_hubs(cls, drone_map: DroneMap) -> dict[str, Hub]:
        """Create a hub for the start hub, every hub, and the end hub.

        Args:
            drone_map (DroneMap): The validated map.

        Returns:
            dict[str, hub]: hubs keyed by hub name, in the order start,
                intermediate hubs, end.
        """
        hubs: dict[str, Hub] = {}

        hubs[drone_map.start.name] = drone_map.start
        for hub in drone_map.hubs:
            hubs[hub.name] = hub
        hubs[drone_map.end.name] = drone_map.end
        return hubs

    @staticmethod
    def _build_adjacency(hubs: dict[str, Hub],
                         connections: list[Connection]) \
            -> dict[str, list[str]]:
        """Build the neighbour lists of every hub.

        Each connection is added in both directions. Connections that
        name an unknown hub are skipped silently.

        Args:
            hubs (dict[str, hub]): hubs keyed by name.
            connections (list[Connection]): Connections to add.

        Returns:
            dict[str, list[str]]: Neighbour names for every hub name.
                hubs without connections map to an empty list.
        """
        adjacency: dict[str, list[str]] = {name: [] for name in hubs}

        for conn in connections:
            if conn.name1 in hubs and conn.name2 in hubs:
                adjacency[conn.name1].append(conn.name2)
                adjacency[conn.name2].append(conn.name1)
        return adjacency

    @staticmethod
    def _build_link_caps(connections: list[Connection]) \
            -> dict[frozenset[str], int]:
        """Map each connection to its maximum link capacity.

        Args:
            connections (list[Connection]): Connections to read.

        Returns:
            dict[frozenset[str], int]: `max_link_capacity` keyed by the
                unordered pair of hub names. If a pair appears twice,
                the last connection wins.
        """
        link_caps = dict()

        for conn in connections:
            tmp_set = frozenset({conn.name1, conn.name2})
            link_caps.update({tmp_set: conn.metadata.max_link_capacity})
        return link_caps

    def valid_neighbours(self, name: str) -> list[str]:
        """Return the neighbours of a hub that can be entered.

        Args:
            name (str): Name of the hub.

        Returns:
            list[str]: Neighbour names, excluding blocked hubs.
        """
        return [x for x in self._adjacency[name] if not self.is_blocked(x)]

    def all_neighbours(self, name: str) -> list[str]:
        """Return every neighbour of a hub, including blocked ones.

        Args:
            name (str): Name of the hub.

        Returns:
            list[str]: A copy of the neighbour list, so changing it does
                not affect the graph.
        """
        return list(self._adjacency[name])

    def is_blocked(self, name: str) -> bool:
        """Check whether a hub is of type `hubType.BLOCKED`.

        Returns:
            bool: `True` if the hub is blocked, otherwise `False`.
        """
        if self._hubs[name].metadata.hub_type == HubType.BLOCKED:
            return True
        return False

    def cost(self, name: str) -> int:
        """Return the cost of entering a hub."""
        hub = self._hubs[name].metadata.hub_type

        if hub == HubType.BLOCKED:
            raise GraphError("Algorithm tried to enter "
                             f"blocked hub '{name}'")
        elif hub not in HUB_COST:
            raise GraphError(f"Algorithm tried to enter '{name}', "
                             f"a hub of type '{hub.value}'")
        return HUB_COST[hub]

    def check_hub_priority(self, name: str) -> int:
        """Return a sort key that favours priority hubs."""
        if self._hubs[name].metadata.hub_type == HubType.PRIORITY:
            return -1
        return 0

    def hub_capacity(self, name: str) -> int:
        """Return the maximum number of drones a hub can hold."""
        return self._hubs[name].metadata.max_drones

    def link_capacity(self, a: str, b: str) -> int:
        """Return the capacity of the link between two hubs.

        Raises:
            GraphError: If either name is not a hub of this graph.
            GraphError: If the two hubs are not connected.
        """
        for name in (a, b):
            if name not in self._hubs:
                raise GraphError(f"Unknown hub '{name}'")

        key = frozenset({a, b})
        if key not in self._link_caps:
            raise GraphError(f"No link found between '{a}' and '{b}'")
        return self._link_caps[key]
