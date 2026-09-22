# Build-in modules
from random import Random
from typing import Callable, Optional, Any, TypeAlias

# Local modules
from ..classes import Maze
from .kruskal import Kruskal
from .dfs import DFS

AnimType: TypeAlias = Optional[Callable[[Any, bool], None]]


def build_kruskal_maze(maze: Maze, rng: Random,
                       animation: AnimType = None) -> None:
    """Build the maze using the kruskal algorithm."""
    Kruskal(maze, rng).construct(animation)


def build_dfs_maze(maze: Maze, rng: Random,
                   animation: AnimType = None) -> None:
    """Build the maze using the dfs algorithm."""
    DFS(maze, rng).construct(animation)
