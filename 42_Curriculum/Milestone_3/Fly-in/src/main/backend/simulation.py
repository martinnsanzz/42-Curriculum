# Built-in modules
from typing import Callable

# Local modules
from .drone import Drone, DroneState, Move
from .graph import Graph
from .pathfinder import PathFinder
from ..exceptions import SimulationError

StepCallback = Callable[[int, list[Move]], None]

class Simulation:
    def __init__(self, graph: Graph, pathfinder: PathFinder) -> None:
        self.pathfinder = pathfinder
        self.graph = graph
        self.distances = self.pathfinder.compute_distance()
        self.drones = self.init_drones(graph._nb_drones)
        self.total_turns = 0

    @staticmethod
    def init_drones(nb_drones: int) -> list[Drone]:
        drones: list[Drone] = []

        for i in range(0, nb_drones):
            drones.append(Drone(
                id=i+1,
                start_x=0,
                start_y=0,
                start_pos="start"
            ))
        return drones

    def all_arrived(self) -> bool:
        return all(d.state == DroneState.FINISH for d in self.drones)

    def run(self, on_step: StepCallback) -> int:
        while not self.all_arrived():
            moves = self.step()
            if not moves:
                    raise SimulationError("No drone can move: deadlock")
            self.total_turns += 1
            on_step(self.total_turns, moves)
        return self.total_turns

    def step(self) -> list[Move]:
        moves: list[Move] = []

        for drone in self.drones:
            moves.append(Move(id=drone.id,
                         zone=self.graph._zones[drone.curr_zone],
                         connection=None))

        return moves