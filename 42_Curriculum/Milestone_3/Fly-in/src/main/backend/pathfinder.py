# Local modules
from collections import deque

# Built-in modules
from .graph import Graph

def bfs(graph: Graph, start: str) -> list[str]:
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


def find_path(graph: Graph, start: str, end: str) -> list[str]:
    path = [start]