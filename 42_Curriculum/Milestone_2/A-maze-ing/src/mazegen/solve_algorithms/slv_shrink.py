# Built-in modules
import time
from typing import Optional

# Local modules
from ..classes import Cell, Maze, Flag
from .slv_base import Maze_Solve, Anim_func


class Maze_Solve_Shrink(Maze_Solve):
    """
    Implementation of the shrink solver
    This is an original solver
    This maze solver only works on perfect mazes but due to its
    simplicity it can be super duper performant.
    """
    def animate(self, anim_func: Optional[Anim_func] = None) -> None:
        """
        Animation method wrapping the generic frame render method.
        This method updates the maze cells with information to best visualise
        The process of the pathsolving algorithm
        Args:
            anim_func: A function that renders a given grid of pixels,
                        conforms to the following signature:
                        (grid: list[list[Pixel]], show_solution: bool)
                        and returns None
        """
        if anim_func is None:
            return
        # Display search pattern
        flags: dict[Cell, Flag] = {}
        for frame in self._frames:
            for cell in frame:
                flags[cell] = cell.flag
                if cell.flag not in (Flag.ENTRY, Flag.EXIT):
                    cell.flag = Flag.FOCUS
            grid = super().animate(anim_func)
            for cell in frame:
                cell.flag = flags[cell]
        # Flash solution
        anim_func(grid, True)
        time.sleep(0.2)

    def get_neighbours_valid(self, cell: Cell,
                             valid_cells: set[Cell],
                             strict: bool = False) -> list[Cell]:
        """
        Returns the reachable neighbours of a given cell if they are in
        the set of valid_cells. If strict is enabled it raises an error
        if no neighbours could be found.

        Args:
            cell: Cell to examine
            valid_cells: Set of valid cells to compare neighbours against
            strict: Raise an error if no valid neighbours are found

        Raises:
            RuntimeError: If no valid neighbours are found and strict is True
        """
        nbs = [x for x
               in super().get_neighbours(cell)
               if x in valid_cells]
        if strict and not nbs:
            raise RuntimeError(f"Cell at {cell.pos} "
                               "could not find valid neighbours")
        return nbs

    def solve(self, maze: Maze, entry: tuple[int, int],
              exit: tuple[int, int]) -> str:
        """
        Algorithm to solve a given perfect maze
        Close each dead end until the start and end cells are found
        and there are no more dead ends to close.
        Any open cells left form a direct connection between start and end
        Then simply walk the path
        The sequence of pixels that represent each movement is stored on the
        solution attribute.

        Args:
            maze: Maze object to solve

        Returns:
            String that represents the solution steps, e.g. SEEESSWNW...
        """
        super().solve(maze, entry, exit)
        # Setup all initial values
        self._maze = maze
        start_pos = entry
        end_pos = exit
        start_cell: Cell = maze.cells[start_pos[0]][start_pos[1]]
        end_cell: Cell = maze.cells[end_pos[0]][end_pos[1]]

        dead_cells: set[Cell] = set()
        valid_cells: set[Cell] = set()
        dead_ends = (0b0111, 0b1011, 0b1101, 0b1110)
        valid_cells = {cell
                       for row in maze.cells
                       for cell in row
                       if not cell.locked}
        dead_cells = {cell for cell in valid_cells if cell.walls in dead_ends}
        dead_cells.discard(start_cell)
        dead_cells.discard(end_cell)
        self._frames.append(set(valid_cells))
        valid_cells.difference_update(dead_cells)
        cells_closed: bool = True
        self._frames = []
        # Close each dead end until entry and exit have been found
        # And no more dead ends can be closed
        while cells_closed:
            cells_closed = False
            for cell in set(dead_cells):
                nbs = self.get_neighbours_valid(cell, valid_cells)
                if nbs:
                    nb = nbs[0]
                    if nb is start_cell or nb is end_cell:
                        pass
                    elif len(self.get_neighbours_valid(nb, valid_cells)) == 1:
                        dead_cells.add(nb)
                        valid_cells.discard(nb)
                        cells_closed = True
                dead_cells.discard(cell)
            self._frames.append(set(valid_cells))
        # Prepare the walk on the valid cells (unordered at this point)
        valid_cells.discard(start_cell)
        for cell in valid_cells:
            cell.flag = Flag.SOLUTION
        cell = start_cell
        cell_pixel = self.cell_to_grid(cell.pos)
        self.steps = [cell_pixel]
        moves: list[str] = []
        while cell is not end_cell:
            nbs = self.get_neighbours_valid(cell, valid_cells, True)
            next_cell = nbs[0]
            next_pixel = self.cell_to_grid(next_cell.pos)
            self.steps.append(self.get_wall_pixel(cell_pixel, next_pixel))
            self.steps.append(next_pixel)
            # Now determine the move
            move = self.dir_from_move(cell.pos, next_cell.pos)
            moves.append(move.name.upper())
            cell = next_cell
            cell_pixel = self.cell_to_grid(cell.pos)
            valid_cells.discard(cell)
        start_cell.flag = Flag.ENTRY
        end_cell.flag = Flag.EXIT
        self.solution = "".join(moves)
        return self.solution
