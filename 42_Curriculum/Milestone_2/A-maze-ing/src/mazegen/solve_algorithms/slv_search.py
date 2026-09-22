# Built-in modules
import time
from typing import Optional

# Local modules
from ..classes import Cell, Maze, Flag
from .slv_base import Maze_Solve, Anim_func


class Maze_Solve_Search(Maze_Solve):
    """
    Pathsolver class to implement pathsolving based on the following idea:
    The entry cell starts searching for the exit and keeps track of all
    possible options. If a deadend is encountered or a path loops onto another
    already existing path that eliminates it from the possible options.
    """
    def animate(self, anim_func: Optional[Anim_func]) -> None:
        """
        Animation method wrapping the generic frame render method.
        This method updates the maze cells with information to best visualise
        The process of the pathsolving algorithm
        In this implementation it displays all paths that
        are currently examined

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
        Returns:
            Solution of encoded as movements into directions (NENEENW...)
        """
        super().solve(maze, entry, exit)
        # Setup all initial values
        self._maze = maze
        start_pos = entry
        end_pos = exit

        cells = maze.cells
        end_cell = cells[end_pos[0]][end_pos[1]]
        cell = cells[start_pos[0]][start_pos[1]]
        paths: list[list[Cell]] = [[cell]]
        visited_cells = {cell}
        self._frames = []
        found_goal = False
        # Start the search
        while not found_goal:
            active_cells: set[Cell] = set()
            for path in list(paths):
                leading_cell = path[-1]
                visited_cells.add(leading_cell)
                nbs = [x for x in self.get_neighbours(leading_cell)
                       if x not in visited_cells]
                for nb in nbs:
                    new_path = list(path)
                    new_path.append(nb)
                    paths.append(new_path)
                    active_cells.update(set(new_path))
                    if nb is end_cell:
                        found_goal = True
                        break
                # Remove the source path
                paths.remove(path)
            self._frames.append(active_cells)

        path = [x for x in paths if end_cell in x][0]
        self._frames.append(set(path))
        self.walk_path(path)
        return self.solution
