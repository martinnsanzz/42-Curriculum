# Local modules
from math import inf
import heapq

# Built-in modules
from .graph import Graph
from ..exceptions import GraphError

class PathFinder:
    def __init__(self, graph: Graph) -> None:
        self.graph = graph


    def compute_distance(self) -> dict[str, int]:
        heap: list[tuple[int, str]] = [(0, self.graph.end)]
        distance: dict[str, int] = {self.graph.end: 0}

        while heap:
            dist, name = heapq.heappop(heap)

            if dist > distance.get(name, inf):
                continue

            # Going back from `name`, a drone moving forward enters `name`
            step = self.graph.cost(name)

            for n in self.graph.valid_neighbours(name):
                new_d = dist + step

                if new_d < distance.get(n, inf):
                    distance[n] = new_d
                    heapq.heappush(heap, (new_d, n))

        if self.graph.start not in distance:
            raise GraphError("Starting point is unreachable\n")
        return distance

    def shortest_path(self) -> tuple[list[str], float]:
        distance: dict[str, float] = {self.graph.start: 0}
        came_from: dict[str, str] = {}
        queue: list[tuple[float, str]] = [(0, self.graph.start)]

        while queue:
            cur_d, cur_n = heapq.heappop(queue)

            if cur_d > distance.get(cur_n, inf):
                continue
            if cur_n == self.graph.end:
                break

            for n in self.graph.valid_neighbours(cur_n):
                new_d = cur_d + self.graph.cost(n)

                if new_d < distance.get(n, inf):
                    distance[n] = new_d
                    came_from[n] = cur_n
                    heapq.heappush(queue, (new_d, n))

        if self.graph.end not in distance:
            raise GraphError("End point is unreachable\n")

        path = [self.graph.end]
        while path[-1] != self.graph.start:
            path.append(came_from[path[-1]])
        path.reverse()
        return path, distance[self.graph.end]