# Built-in modules
from random import Random

# Local modules
from ..classes import Cell, Dir, Maze
from .maze_gen import Maze_Gen, Anim_func


class Kruskal(Maze_Gen):
    """ This class construcs a maze following the
    kruskal algorithm with a few tweaks.

    Attributes:
        connected: A list that contains all the cells
                   that are already connected.
        all_cells: A list of all cells that are not locked
        cell_cnt: The total amount of unlocked cells
    """
    def __init__(self, maze: Maze, rng: Random) -> None:
        """
        Initializes the Kruskal algorithm which implements
        additional attributes and variations of the generic
        Maze_gen class

        Args:
            maze (Maze): Maze object to modify.
        """
        super().__init__(maze, rng)
        self.connected: list[set[Cell]] = []
        self.all_cells: list[Cell] = list()
        cell: Cell
        self.cell_cnt: int = 0
        for row in self._maze.cells:
            for cell in row:
                if not cell.locked:
                    self.all_cells.append(cell)
                    self.connected.append({cell})
                    self.cell_cnt += 1

    def break_wall_path(self, cell: Cell, path: set[Cell],
                        direction: Dir, tar_cell: Cell) -> None:
        """Override on the generic break_wall method.

        This implements the generic method and merges the paths of the two
        cells that are connected.

        Args:
            cell (Cell) Current cell in focus.
            direction (Dir) Wall to break in the current cell.
            tar_cell (Cell) Cell to open a path to.
        """
        self.break_wall(cell, direction, tar_cell)
        for i, p in enumerate(self.connected):
            if tar_cell in p:
                tar_path = self.connected.pop(i)
                break
        path.update(tar_path)

    def get_neighbours_path(self, cell: Cell,
                            path: set[Cell]) -> list[tuple[Dir, Cell]]:
        """Override on the generic get_neighbours method.

        Get all the neighbours and filter out any that are already
        connected to the cell in focus

        Args:
            cell (Cell) Cell object to be checked.

        Returns:
            A list of tuples with (direction, cell).
        """
        nbs = self.get_neighbours(cell)
        nbs = [x for x in nbs if x[1] not in path]
        return nbs

    def get_connected(self, cell: Cell) -> set[Cell]:
        """Gets the path of connected cells

        Args:
            cell (Cell) The cell to be checked.

        Returns:
            A set of all cells connected to the given cell

        Raises:
            Runtime Error if the cell can't be found
        """
        for path in self.connected:
            if cell in path:
                return path
        raise RuntimeError("Cell could not be found. "
                           "Error with internal logic")

    def construct(self, animation: Anim_func | None = None) -> None:
        """
        Modifies a fresh maze grid using the kruskal algorithm.
            1. Make a list of all the to-be-connected cells and shuffle it
            2. Take the last item
            3. Open up a wall to a neighbour cell that its not yet connected to
            4. If that cell has neighbours to connect to go to step 3
               otherwise go to step 2
            5. Once finish check if the maze need to be perfect or imperfect.

        Args:
            anim_func: A function that renders a given grid of pixels,
                       conforms to the following signature:
                       (grid: list[list[Pixel]])
                       and returns None
        """

        cell_pool: list[Cell] = list(self.all_cells)
        self._rng.shuffle(cell_pool)
        cell = cell_pool.pop()
        tar_cell: Cell = cell
        path: set[Cell] = self.get_connected(cell)
        i: int = 1
        nbs: list[tuple[Dir, Cell]]
        while i < self.cell_cnt:
            nbs = self.get_neighbours_path(cell, path)
            if not nbs:
                if cell_pool:
                    cell = cell_pool.pop()
                else:
                    cell = self._rng.choice([x for x in self.all_cells
                                            if x not in path])
                path = self.get_connected(cell)
                continue

            tar_dir, tar_cell = self._rng.choice(nbs)
            self.break_wall_path(cell, path, tar_dir, tar_cell)
            if animation:
                super().play_animation(cell, animation)
            cell = tar_cell
            i += 1

        if not self._maze._perfect:
            self.imperfect_maze(animation)
