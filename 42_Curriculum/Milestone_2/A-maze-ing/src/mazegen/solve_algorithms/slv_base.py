# Built-in modules
from abc import abstractmethod, ABC
from typing import Callable, TypeAlias, Optional
import time

# Local modules
from ..classes import Cell, Dir, Maze, Pixel, Flag


Anim_func: TypeAlias = Callable[[list[list[Pixel]] | None, bool], None]


class Maze_Solve(ABC):
    """Abstract Maze solver class.

    This class serves as a blueprint to all maze solvers.
    It provides basic shared methods and attributes

    Attributes:
        solution: A list of pixel positions in order from entry to exit
        steps: A string representing the moves to reach the exit
        _frames: A list of sets of cells which to highlight for animation
        _time: A time attribute for animation purposes
        _target_speed: The ideal refresh rate per (0.1 is 10fps)
        _maze: The maze which is to solve
    """
    def __init__(self) -> None:
        """
        Initialise base values.
        """
        self.solution: str = ""
        self.steps: list[tuple[int, int]] = []
        self._frames: list[set[Cell]] = []
        self._time: float = 0
        self._target_speed: float = 0.03
        self._maze: Maze

    @abstractmethod
    def animate(self, anim_func: Optional[Anim_func]) \
            -> Optional[list[list[Pixel]]]:
        """
        Basic method to be called by specialised overrides to render
        a frame of the current solve process.
        Args:
            anim_func: A function that renders a given grid of pixels,
                       conforms to the following signature:
                       (grid: list[list[Pixel]], show_solution: bool)
                       and returns None
        Returns:
            The 2D list of pixels for further usage
        """
        if anim_func is None:
            return None
        grid = self._maze._get_maze()
        now: float = time.time()
        time.sleep(max(self._target_speed - (now - self._time), 0))
        self._time = now
        anim_func(grid, False)
        return grid

    def get_neighbours(self, cell: Cell) -> list[Cell]:
        """
        Get all the connected neighbours the current cell.

        Args:
            cell: Cell object to be checked.

        Returns:
            A list of cells.
        """
        row, col = cell.pos
        nbs: list[Cell] = []
        nb: Cell
        for direction in list(Dir):
            i, j = Dir.offset(direction)
            if cell.get_wall(direction):
                continue
            elif ((row + i) == -1) or ((col + j) == -1):
                continue
            try:
                nb = self._maze.cells[row + i][col + j]
            except IndexError:
                continue
            nbs.append(nb)
        return nbs

    def walk_path(self, path: list[Cell]) -> None:
        """
        Translates a list of cells representing a path (from entry to exit)
        into a list of steps (pixel positions) and a solution (str)
        which are stored as attributes

        Args:
            path: List of cells each representing one move
        """
        for _cell in path:
            if _cell.flag not in (Flag.ENTRY, Flag.EXIT):
                _cell.flag = Flag.SOLUTION
        moves: list[str] = []
        cell: Cell = path.pop(0)
        pixel_pos: tuple[int, int] = self.cell_to_grid(cell.pos)
        self.steps = [pixel_pos]
        while path:
            next_cell = path.pop(0)
            next_pixel_pos = self.cell_to_grid(next_cell.pos)
            self.steps.append(self.get_wall_pixel(pixel_pos, next_pixel_pos))
            self.steps.append(next_pixel_pos)
            # Now determine the move
            move = self.dir_from_move(cell.pos, next_cell.pos)
            moves.append(move.name.upper())
            cell = next_cell
            pixel_pos = self.cell_to_grid(cell.pos)
        self.solution = "".join(moves)

    @staticmethod
    def cell_to_grid(pos: tuple[int, int]) -> tuple[int, int]:
        """
        Static method that converts the x, y coordinates of a cell to the
        x, y coordinates on a pixelgrid

        Args:
            pos: row and column of the cell in the 2D cell list
        Returns:
            A tuple of row and column of the cell in the 2D list of pixels
        """
        return (1 + (pos[0] * 2), 1 + (pos[1] * 2))

    @staticmethod
    def get_wall_pixel(pos_1: tuple[int, int],
                       pos_2: tuple[int, int]) -> tuple[int, int]:
        """
        Static method that given two pixels of adjacent cells
        returns the position of the pixel representing the wall
        inbetween them

        Args:
            pos_1: Position vector of the first cell
            pos_2: Position vector of the second cell

        Returns:
            Position of the pixel representing the wall
            between pos_1 and pos_2
        """
        adj_pos_1 = int((pos_1[0] + pos_2[0]) / 2)
        adj_pos_2 = int((pos_1[1] + pos_2[1]) / 2)
        return (adj_pos_1, adj_pos_2)

    @staticmethod
    def dir_from_move(start: tuple[int, int],
                      end: tuple[int, int]) -> Dir:
        """
        Static method that given a start pos determines
        the direction of movement required to reach the end pos
        The two positions need to be adjacent

        Args:
            start: Position vector of the start cell
            end: Position vector of the end cell

        Returns:
            Direction of movement from start to end

        Raises:
            RuntimeError: If start and end are not adjacent
        """
        diff: tuple[int, int] = (int(end[0] - start[0]),
                                 int(end[1] - start[1]))
        for dir in list(Dir):
            if diff == Dir.offset(dir):
                return dir
        raise RuntimeError("Direction could not be determined. "
                           f"Start {start} and end {end} are "
                           "possibly not adjacent")

    @abstractmethod
    def solve(self, maze: Maze, entry: tuple[int, int],
              exit: tuple[int, int]) -> str:
        """
        Implement the solving algorithm

        Args:
            maze: Maze object to solve.
            entry: Entry point of maze.
            exit: Exit point of maze.
        """
        if entry == exit:
            raise RuntimeError("Entry and exit shouldnt be equal")
        if (entry[0] >= maze._height) or (entry[0] < 0):
            raise RuntimeError("Entry point is out of bounds")
        if (entry[1] >= maze._width) or (entry[1] < 0):
            raise RuntimeError("Entry point is out of bounds")
        if (exit[0] >= maze._height) or (exit[0] < 0):
            raise RuntimeError("Exit point is out of bounds")
        if (exit[1] >= maze._width) or (exit[1] < 0):
            raise RuntimeError("Exit point is out of bounds")
        if maze._cells[entry[0]][entry[1]].locked:
            raise RuntimeError("Entry is a pattern cell")
        if maze._cells[exit[0]][exit[1]].locked:
            raise RuntimeError("Exit is a pattern cell")
        maze._cells[entry[0]][entry[1]].flag = Flag.ENTRY
        maze._cells[exit[0]][exit[1]].flag = Flag.EXIT
        return ""
