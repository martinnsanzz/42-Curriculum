# Built-in modules
from typing import Callable, Optional
from collections import defaultdict

# Local modules
from .drone import Drone, DroneState, Move
from .graph import Graph
from .pathfinder import PathFinder
from ..exceptions import SimulationError

StepCallback = Callable[[int, list[Move]], None]


class Simulation:
    """Simulate drones moving from the start hub to the end hub of a graph.

    The simulation advances in discrete turns. On each turn, drones already
    travelling along a multi-turn connection progress, then idle drones try
    to pick the next hub on a shortest path, subject to hub and link
    capacity. The simulation ends when every drone reaches the end hub.

    Attributes:
        graph: Graph describing hubs, links, costs and capacities.
        pathfinder: Pathfinder used to compute distances to the end hub.
        occupancy: Number of drones currently counted in each hub, keyed
            by hub name.
        distances: Distance from each hub to the end hub, as returned by
            `PathFinder.compute_distance()`. [INFERRED]
        drones: All drones taking part in the simulation.
        total_turns: Number of turns simulated so far.
    """
    def __init__(self, graph: Graph, pathfinder: PathFinder) -> None:
        """Initialize the simulation state and place all drones at the start.

        Args:
            graph: Graph describing the hubs and connections. Its drone
                count is read from `graph._nb_drones`.
            pathfinder: Pathfinder used to compute hub distances.
        """
        self.graph = graph
        self.occupancy: defaultdict[str, int] = defaultdict(int)
        self.distances = pathfinder.compute_distance()
        self.drones = self._init_drones(graph._nb_drones)
        self.total_turns = 0

    def _init_drones(self, nb_drones: int) -> list[Drone]:
        """Create the drones and register them in the start hub."""
        drones: list[Drone] = []

        for i in range(0, nb_drones):
            drones.append(Drone(
                id=i+1,
                start_x=0,
                start_y=0,
                start_hub="start"
            ))
        self.occupancy[self.graph.start] = len(drones)
        return drones

    def run(self) -> list[list[Move]]:
        """Run the simulation until all drones have reached the end hub.

        Returns:
            A 2D list containing all the drones moves per step.

        Raises:
            SimulationError: If a turn produces no moves while drones are
                still unfinished (deadlock).
        """
        total_moves: list[list[Move]] = list(list())

        while not self._all_arrived():
            step_moves = self._step()
            if not step_moves:
                raise SimulationError("No drone can move: deadlock")
            total_moves.append(step_moves)
        return total_moves

    def _step(self) -> list[Move]:
        """Advance the simulation by one turn.

        Drones in transit progress first and arrive when their remaining
        turns reach zero. Idle, unfinished drones then try to start a new
        move toward the next hub.

        Returns:
            The moves made this turn, sorted by drone id.

        Note:
            If a Drone has moved to a target after waiting
            on a connection, it will be added to a not_use list to avoid
            moving a drone twice in one turn.
        """
        moves: list[Move] = []
        link_use: defaultdict[frozenset[str], int] = defaultdict(int)
        not_use: list[Drone] = []

        for drone in self.drones:
            if drone.target_hub is None:
                continue

            pair = frozenset({drone.curr_hub, drone.target_hub})
            link_use[pair] += 1
            drone.turns_left -= 1

            if drone.turns_left == 0:
                self._arrive(drone, drone.target_hub)
                moves.append(self._create_move(drone))
                not_use.append(drone)

        for drone in self.drones:
            if drone.target_hub is not None or \
                drone.state == DroneState.FINISH or \
                    drone in not_use:
                continue
            nxt = self._pick_next(drone, link_use)
            if nxt is None:
                continue

            pair = frozenset({drone.curr_hub, nxt})
            link_use[pair] += 1
            self.occupancy[drone.curr_hub] -= 1
            self.occupancy[nxt] += 1

            cost = self.graph.cost(nxt)
            if cost == 1:
                self._arrive(drone, nxt)
                moves.append(self._create_move(drone))
            else:
                drone.target_hub = nxt
                drone.turns_left = cost - 1
                moves.append(self._create_move(drone))
        moves.sort(key=lambda m: m.id)
        return moves

    def _all_arrived(self) -> bool:
        """Check whether every drone has finished."""
        return all(d.state == DroneState.FINISH for d in self.drones)

    def _arrive(self, drone: Drone, hub: str) -> None:
        """Place a drone in a hub and finish it if the hub is the end"""
        drone.curr_hub = hub
        drone.target_hub = None
        if hub == self.graph.end:
            drone.set_state(DroneState.FINISH)

    def _pick_next(self, drone: Drone,
                   link_use: defaultdict[frozenset[str], int]) \
            -> Optional[str]:
        """Choose the next hub for an idle drone.

        Candidates are neighbours that lie on a shortest path to the end
        hub, ordered by `Graph.check_hub_priority`. The first candidate
        with free hub capacity and free link capacity is selected.

        Args:
            drone: Drone looking for its next hub.
            link_use: Number of drones using each link this turn, keyed by
                the unordered pair of hub names. Not modified.

        Returns:
            The name of the chosen hub, or None if no candidate is
            available this turn.
        """
        here = drone.curr_hub
        candidates = [n for n in self.graph.valid_neighbours(here)
                      if self.graph.cost(n) + self.distances[n]
                      == self.distances[here]]
        candidates.sort(key=self.graph.check_hub_priority)

        for n in candidates:
            if self.occupancy[n] >= self.graph.hub_capacity(n):
                continue
            if link_use[frozenset({here, n})] >= \
               self.graph.link_capacity(here, n):
                continue
            return n
        return None

    def _create_move(self, drone: Drone) -> Move:
        """Build the move record describing a drone's action this turn.

        Args:
            drone: Drone that has just moved or started moving.

        Returns:
            A `Move` with the connection name if the drone is still in
            transit, otherwise a `Move` with the hub it arrived in.
        """
        if drone.target_hub is not None:
            return Move(
                id=drone.id,
                hub=None,
                connection=drone.connection_name
            )
        return Move(
            id=drone.id,
            hub=self.graph._hubs[drone.curr_hub],
            connection=None
        )
