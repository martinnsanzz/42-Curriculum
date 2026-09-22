# Built-in modules
from random import Random

# Local modules
from ..classes import Cell, Dir, Maze
from .maze_gen import Maze_Gen, Anim_func


class DFS(Maze_Gen):
    """ This class construcs a maze following the dfs algorithm.

    Attributes:
        visited_cells (set[Cell]): A set that contains all visited cells
                                    while the algorithm runs.
    """
    def __init__(self, maze: Maze, rng: Random):
        """Initializes dfs class.

        Args:
            maze (Maze): Maze object to modify.
        """
        super().__init__(maze, rng)
        self.visited_cells: set[Cell] = set()

    def get_neighbours(self, cell: Cell) -> list[tuple[Dir, Cell]]:
        """Override on the generic get_neighbours method.

        Adds another filter specific to this algorithm were it doesnt consider
        a neighbour if its already within the visited_cells[].
        Args:
            cell (Cell): Cell to check neighbours from

        Returns:
            A list of tuples with (direction, cell).
        """
        nbs = super().get_neighbours(cell)

        nbs = [x for x in nbs if not x[1] in self.visited_cells]
        return nbs

    def construct(self, animation: Anim_func | None = None) -> None:
        """
        Modifies a fresh maze grid using the dfs algorithm.
            1. Selects a random cell of the maze to start from.
            2. Add this cell to the stack[] (for backtracking) and to the
                visited_cell[].
            3. While theres cells on the stack keep looping.
                3a. If cell has neighbours append current_cell to stack and
                    select the next cell. Add next_cell to visited cells and
                    break the walls between the 2 cells.
                3b. If no neighbours but theres still cells on the stack make
                    current_cell equal to the last cell added to the stack
                    (backtracking) and remove this cell from the stack.
            4. Once finish check if the maze need to be perfect or imperfect.
        """
        current_cell: Cell = self.select_rand_cell()
        stack: list[Cell] = []

        self.visited_cells.add(current_cell)
        stack.append(current_cell)

        while stack:
            cell_neighbours = self.get_neighbours(current_cell)
            if animation:
                super().play_animation(current_cell, animation)

            if cell_neighbours:
                stack.append(current_cell)
                next_cell: tuple[Dir, Cell] = self._rng.choice(cell_neighbours)
                self.visited_cells.add(next_cell[1])
                self.break_wall(current_cell, next_cell[0], next_cell[1])
                current_cell = next_cell[1]
            elif stack:
                current_cell = stack[-1]
                stack.pop()

        if not self._maze._perfect:
            self.imperfect_maze(animation)
