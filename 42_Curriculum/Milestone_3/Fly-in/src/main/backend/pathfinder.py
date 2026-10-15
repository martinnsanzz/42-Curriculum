# Local modules
from collections import deque
from math import inf

# Built-in modules
from .graph import Graph

def bfs_traversal(graph: Graph, start: str) -> list[str]:
    visited = [start]
    result = list()
    queue = deque()

    visited[0] = start
    queue.append(start)

    while queue:
        curr = queue.popleft()
        result.append(curr)

        neighbours = graph.all_neighbours(curr)
        for x in neighbours:
            if x not in visited:
                visited.append(x)
                queue.append(x)
    return result


def dijkstra_alg(graph: Graph, start: str, end: str) -> list[str]:
    dist: dict[str, int] = {hub: inf for hub in graph._hubs}
    visited: dict[str, bool] = {hub: False for hub in graph._hubs}

    dist[start] = 0
    
