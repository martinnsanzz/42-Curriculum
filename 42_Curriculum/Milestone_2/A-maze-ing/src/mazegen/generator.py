# Build-in modules
from random import Random
from typing import Optional, Callable, Any, TypeAlias

# Local modules
from .classes import Maze
from .solve_algorithms import Maze_Solve, Maze_Solve_Find, \
                              Maze_Solve_Search, Maze_Solve_Shrink
from .gen_algorithms import build_kruskal_maze, build_dfs_maze

AnimType: TypeAlias = Optional[Callable[[Any, bool], None]]


class MazeGenerator():
    """Build and solve mazes using a configurable generation algorithm.

    Instantiate with the maze dimensions and the desired generation
    algorithm, then call `generate()` to produce a `Maze` and `solve()`
    to get a string with the directions to solve the maze and a
    list of tuples with the cordinates of each move.

    Attributes:
        _width (int): Number of columns in the generated maze.
        _height (int): Number of rows in the generated maze.
        _build_algorithm (str): Generation algorithm identifier
            ("kruskal" or "dfs").
        _solve_algorithm (str): Solve algorithm identifier
            ("find", "search" or "find only if _perfect == True").
        _perfect (bool): Whether the maze is generated as "perfect"
            (exactly one path between any two cells, no loops).
        _rng (Random): Seeded random number generator used for
            deterministic or randomized generation.
        _pattern (str): Pattern to draw on middle of maze
            ("42", "square", "star" or nothing (default))
        _animation (Callable): Animation function (Only use for curses)
        _solver (Maze_Solve): Maze solve generation class

    Notes:
        If the provided algorithm isnt on the supported list it will provide a
        maze with all walls up
    """
    def __init__(self, width: int, height: int, build_algorithm: str,
                 solve_algorithm: str, perfect: bool,
                 seed: Optional[int] = None, pattern: Optional[str] = None,
                 animation: AnimType = None) -> None:
        """Initialize generator parameters.

        Args:
            width: Number of columns in the maze.
            height: Number of rows in the maze.
            build_ algorithm: Generation algorithm to use. Supported values
                are "kruskal" and "dfs".
            solve_algorithm: Solve algorithm to use. Supported values are
                "find", "search" or "'find' only if perfect == True"
            perfect: If True, generate a perfect maze (no loops).
            seed: Seed for the random number generator. If None, the
                generator is seeded non-deterministically.
            pattern: Pattern shown on middle of maze. Supported values are
                "42", "star", "square" or nothing.
            animation: Leave empty (Only for curses)
        """
        self._width = width
        self._height = height
        self._build_algorithm = build_algorithm
        self._solve_algorithm = solve_algorithm
        self._perfect = perfect
        self._rng = Random(seed)
        self._pattern = pattern
        self._animation = animation
        self._solver: Optional[Maze_Solve] = None

        if solve_algorithm == "shrink" and not self._perfect:
            raise ValueError("Can't use 'shrink' with imperfect maze")

    def generate(self, animate: bool = False) -> Maze:
        """Generate a maze using the configured algorithm.

        Args:
            animate: Keep False unless using curses.

        Returns:
            Maze: A new maze built with the configured algorithm. If
            `algorithm` is not `"kruskal"` or `"dfs"`, an empty,
            unmodified `Maze` is returned (no passages carved).

        Notes:
            Once the maze is created use method .get_print_string() to print
            the maze to the terminal.
            If using curses it and (animate == True) it will animate the
            build of the maze.
        """
        anim_func = None
        if animate:
            anim_func = self._animation

        if self._width < 2 or self._height < 2:
            raise RuntimeError("Size of maze cant be smaller than 2x2. "
                               "Increase the size of the maze")

        maze = Maze(self._width, self._height, self._perfect, self._pattern)

        if self._build_algorithm == "kruskal":
            build_kruskal_maze(maze, self._rng, anim_func)
        elif self._build_algorithm == "dfs":
            build_dfs_maze(maze, self._rng, anim_func)
        return maze

    def set_animation(self, animation: AnimType = None) -> None:
        """
        Set or update the animation function

        Args:
            animation: Animation function for visualisation
        """
        self._animation = animation

    def solve(self, maze: Maze, entry: tuple[int, int],
              exit: tuple[int, int]) \
            -> tuple[str, list[tuple[int, int]]]:
        """Solve a maze between two points.

        Args:
            maze: Maze to solve.
            entry: (row, col) coordinate of the starting cell. Min value (0, 0)
            exit: (row, col) coordinate of the target cell.
                  Max value (width - 1, height - 1)

        Returns:
            tuple[str, list[tuple[int, int]]]: Solution identifier and
            the ordered list of coordinates from `entry` to `exit`.

        Raises:
            If entry and exit values aren't correct.

        Notes:
            Animates the solver algorithm and can only be seeing if curses if
            being used.
        """
        if self._solver:
            self._solver.animate(self._animation)
            return self._solver.solution, self._solver.steps

        if self._solve_algorithm == "shrink":
            self._solver = Maze_Solve_Shrink()
        elif self._solve_algorithm == "find":
            self._solver = Maze_Solve_Find()
        elif self._solve_algorithm == "search":
            self._solver = Maze_Solve_Search()

        try:
            if self._solver:
                self._solver.solve(maze, entry, exit)
                self._solver.animate(self._animation)
                return self._solver.solution, self._solver.steps
        except BaseException as e:
            raise BaseException(str(e))
        raise NotImplementedError
