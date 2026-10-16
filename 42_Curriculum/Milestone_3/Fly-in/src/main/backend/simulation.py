# Built-in modules

# Local modules
from .drone import Drone, DroneState
from .graph import Graph
from .pathfinder import PathFinder
from ..exceptions import SimulationError

class Simulation:
    def __init__(self, graph: Graph, pathfinder: PathFinder) -> None:
        self.pathfinder = pathfinder
        self.graph = graph
        self.distances = self.pathfinder.compute_distance()
        self.available_drones = self.init_drones(graph._nb_drones)
        print(self.distances)
        print(self.available_drones)

    def init_drones(self, nb_drones: int) -> list[Drone]:
        drones: list[Drone] = []

        for i in range(0, nb_drones):
            drones.append(Drone(
                id=i,
                start_x=0,
                start_y=0,
                start_pos="start"
            ))
        return drones

    def step(self) -> None:
        ...