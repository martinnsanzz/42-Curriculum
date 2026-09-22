# Built-in modules
from typing import Optional

# Local modules
from ..classes import Cell, Maze, Flag
from .slv_base import Maze_Solve, Anim_func


class Maze_Solve_Find(Maze_Solve):
    """
    Pathsolver class to implement pathsolving based on the following idea:
    The exit tells all cells how far away they are from it
    The entry then walks along the cheapest route to the exit
    """
    def animate(self, anim_func: Optional[Anim_func]) -> None:
        """
        Animation method wrapping the generic frame render method.
        This method updates the maze cells with information to best visualise
        The process of the pathsolving algorithm
        In this implementation it flashes each pixel that was recently
        processed without leaving a trail
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
        self._time = 0
        for frame in self._frames:
            for cell in frame:
                if cell not in flags:
                    flags[cell] = cell.flag
                    cell.flag = Flag.FOCUS
            super().animate(anim_func)
            for cell in flags:
                cell.flag = flags[cell]
        super().animate(anim_func)

    def solve(self, maze: Maze, entry: tuple[int, int],
              exit: tuple[int, int]) -> str:
        """
        Solve the given maze
        1) Keep a list of possible paths, each path is stored as a list
        2) The first possible path only contains the starting cell
        3) For all possible paths we do:
            a) Get all the neighbours of the most recent cell
            b) Remove any neighbours that are already included in other paths
            c) Duplicate the current possible path for each
                remaining neighbour with that neigbour added to the end
            d) Remove the current possible path from the list of possible
                paths.
        4) Repeat until one of the paths encounters the end cell

        Args:
            maze: Maze object to solve
        Raises:
            RuntimeError: If a final path is not found
        """
        super().solve(maze, entry, exit)
        # Setup all initial values
        self._maze = maze
        start_pos = entry
        end_pos = exit

        exit_cell = maze.cells[end_pos[0]][end_pos[1]]
        entry_cell = maze.cells[start_pos[0]][start_pos[1]]
        cost = {exit_cell: 0}
        enough = False
        self._frames = [{exit_cell}]
        # Assign a cost to each cell until the entry is found
        while not enough:
            new_cells = set()
            for cell in set(cost.keys()):
                nbs = self.get_neighbours(cell)
                for nb in nbs:
                    if nb not in cost:
                        cost[nb] = cost[cell] + 1
                        new_cells.add(nb)
                    if nb is entry_cell:
                        enough = True
            self._frames.append(new_cells)

        # Walk the cheapest path from the entry to the exit
        path = [entry_cell]
        cell = entry_cell
        # cell.flag = Flag.SOLUTION
        while cell is not exit_cell:
            nbs = [x for x in self.get_neighbours(cell) if x in cost]
            cheapest_nb = nbs[0]
            for nb in nbs:
                if cost[nb] < cost[cheapest_nb]:
                    cheapest_nb = nb
            path.append(cheapest_nb)
            cell = cheapest_nb
            # cell.flag = Flag.SOLUTION

        # Translate the path into a solution
        self.walk_path(path)
        return self.solution
