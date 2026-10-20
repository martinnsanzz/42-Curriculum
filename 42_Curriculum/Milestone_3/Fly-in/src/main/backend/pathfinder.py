# Local modules
import heapq

# Built-in modules
from .graph import Graph
from ..exceptions import GraphError

class PathFinder:
    """Compute shortest paths on a weighted graph using Dijkstra's algorithm.

    The cost of a path is the sum of ``graph.cost(node)`` over the nodes
    entered along it. Neighbours are provided by ``graph.valid_neighbours``.

    Args:
        graph: Graph exposing ``start``, ``end``, ``cost(name)`` and
            ``valid_neighbours(name)``.

    Attributes:
        graph: The graph on which distances and paths are computed.
    """
    def __init__(self, graph: Graph) -> None:
        self.graph = graph


    def compute_distance(self) -> dict[str, int]:
        """Compute the cost from every reachable node to the end node.

        Run Dijkstra backwards from ``graph.end``, so each value is the
        total cost of the cheapest route from that node to the end.

        Returns:
            Mapping of node name to its distance from ``graph.end``.
            ``graph.end`` maps to 0.

        Raises:
            GraphError: If ``graph.start`` is not reachable from
                ``graph.end``.
        """
        heap: list[tuple[int, str]] = [(0, self.graph.end)]
        distance: dict[str, int] = {self.graph.end: 0}

        while heap:
            dist, name = heapq.heappop(heap)

            if dist > distance[name]:
                continue

            step = self.graph.cost(name)

            for n in self.graph.valid_neighbours(name):
                new_d = dist + step

                if n not in distance or new_d < distance[n]:
                    distance[n] = new_d
                    heapq.heappush(heap, (new_d, n))

        if self.graph.start not in distance:
            raise GraphError("Starting point is unreachable")
        return distance

    # ------------------------ Not use ------------------------
    # Left behind for visual propuses. This was initial algorithm
    # implementation.
    #
    # def shortest_path(self) -> tuple[list[str], int]:
    #     """Find the fastest path from ``graph.start`` to ``graph.end``.

    #     Run Dijkstra forwards from the start node and stop as soon as the
    #     end node is popped from the queue. The cost of a path is a pair
    #     ``(turns, priority)``, compared in that order. ``priority`` is the
    #     negative count of priority zones entered, so among paths with equal
    #     turns, the one through more priority zones wins.

    #     Returns:
    #         A tuple ``(path, turns)`` where ``path`` is the ordered list of
    #         node names from start to end (both inclusive) and ``turns`` is
    #         the number of turns that path takes.

    #     Raises:
    #         GraphError: If ``graph.end`` is not reachable from
    #             ``graph.start``.
    #     """
    #     start, end = self.graph.start, self.graph.end
    #     distance: dict[str, tuple[int, int]] = {start: (0, 0)}
    #     came_from: dict[str, str] = {}
    #     queue: list[tuple[int, int, str]] = [(0, 0, start)]

    #     while queue:
    #         turns, priority, cur_n = heapq.heappop(queue)

    #         if (turns, priority) > distance[cur_n]:
    #             continue
    #         if cur_n == end:
    #             break

    #         for n in self.graph.valid_neighbours(cur_n):
    #             new_d = (
    #                 turns + self.graph.cost(n),
    #                 priority + self.graph.check_hub_priority(n),
    #             )

    #             if n not in distance or new_d < distance[n]:
    #                 distance[n] = new_d
    #                 came_from[n] = cur_n
    #                 heapq.heappush(queue, (new_d[0], new_d[1], n))

    #     if end not in distance:
    #         raise GraphError("End point is unreachable")

    #     path = [end]
    #     while path[-1] != start:
    #         path.append(came_from[path[-1]])
    #     path.reverse()
    #     return path, distance[end][0]