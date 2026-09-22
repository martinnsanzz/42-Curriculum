# Built-in modules
from typing import Optional
from math import ceil

# Local modules
from .pixel_class import Pixel, Flag
from .dir_class import Dir
from .cell_class import Cell


class Maze():
    """This class represents the maze grid based on `config.txt`.

    Attributes:
        maze_config (MazeConfig): A copy of the maze configuration to be used
                                  where needed.
        _width (int): Width of maze.
        _height (int): Height of maze.
        _cells (list[list[Cell]]): A 2D list of the cells from the maze.
        _perfect (bool): If maze needs to be built perfect.
        _pattern (list[list[int]]): Pattern to use inside the maze
    """
    def __init__(self, width: int, height: int, perfect: bool,
                 pattern: Optional[str] = None) -> None:
        """Initialise a new maze based on maze config.

        Args:
            maze_config (MazeConfig): Maze configuration extracted
                                      from `config.txt`.

        Notes:
            Calls random.seed(mazed_condif.seed) to mantain Reproducibility
            across calls.
        """
        self._width = width
        self._height = height
        self._perfect = perfect
        self._pattern = self.Patterns.get(pattern)
        self._cells: list[list[Cell]] = []

        cell: Cell
        for x in range(self._height):
            row: list[Cell] = []
            for y in range(self._width):
                cell = Cell((x, y))
                row.append(cell)
            self._cells.append(row)

    @property
    def cells(self) -> list[list[Cell]]:
        """Returns the 2D list of Cells."""
        return self._cells

    @cells.setter
    def cells(self, values: list[list[int]]) -> None:
        """Set the cells on the maze cells to the given wall value

        Args:
            values (list[list[int]]): 2D array of integer values holding the
                                      value of the walls
        Raises:
            Error if values are bigger than height or width of maze.
        """
        if len(values) != self._height:
            raise RuntimeError("The height of the provided values does "
                               "not match the height of the maze. "
                               f"Given: {len(values)}, "
                               f"expected: {self._height}")
        for y, row in enumerate(self._cells):
            if len(row) != self._width:
                raise RuntimeError("The width of the provided values does"
                                   "not match the width of the maze. "
                                   f"Given: {len(values)}, "
                                   f"expected: {self._width}")
            for x, cell in enumerate(row):
                cell.walls = values[y][x]

    @property
    def size(self) -> tuple[int, int]:
        """The size of the maze in walkable cells."""
        return (self._width, self._height)

    def _get_maze(self) -> list[list[Pixel]]:
        """
        Return the print 2D list of Pixels for the maze.
        These represent not just cells, but also walls and
        serve as basis for visual interpretation
        """
        col_cnt = (self._width * 2) + 1
        row_cnt = (self._height * 2) + 1
        grid: list[list[Pixel]] = []
        for _ in range(row_cnt):
            row_pixel = []
            for __ in range(col_cnt):
                row_pixel.append(Pixel())
            grid.append(row_pixel)

        for row, row_l in enumerate(self._cells):
            grid_row = (row * 2) + 1
            for col, cell in enumerate(row_l):
                flag = cell.flag
                grid_col = (col * 2) + 1
                grid[grid_row][grid_col].add_flag(flag)
                for dir in list(Dir):
                    r_off, c_off = Dir.offset(dir)
                    wall_flag = (flag, Flag.WALL)[cell.get_wall(dir)]
                    if flag is Flag.PATTERN:
                        grid[grid_row + r_off][grid_col + c_off].add_flag(flag)
                    else:
                        grid[grid_row + r_off][grid_col + c_off].\
                            add_flag(wall_flag)
        for row in range(2, (self._height * 2), 2):
            for col in range(2, (self._width * 2), 2):
                wall_pixels = set()
                wall_pixels.add(grid[row - 1][col].read())
                wall_pixels.add(grid[row][col - 1].read())
                wall_pixels.add(grid[row][col + 1].read())
                wall_pixels.add(grid[row + 1][col].read())
                if Flag.SOLUTION in wall_pixels:
                    wall_pixels.discard(Flag.SOLUTION)
                    wall_pixels.add(Flag.EMPTY)
                if len(wall_pixels) == 1:
                    grid[row][col].add_flag(wall_pixels.pop())
        for i in range(row_cnt - 1):
            grid[i][0].add_flag(Flag.WALL)
            grid[i][col_cnt - 1].add_flag(Flag.WALL)
            if i == 0 or i == (row_cnt - 1):
                for j in range(0, col_cnt - 1):
                    grid[i][j].add_flag(Flag.WALL)
        return grid

    def get_print_string(self) -> str:
        """Converts the maze from a list[list[Pixel] to a string.

        Notes:
            This is used to see the maze within the terminal
        """
        pixels = self._get_maze()
        colors = {Flag.EMPTY: "  ",
                  Flag.SOLUTION: "\033[32m██\033[0m",
                  Flag.PATTERN: "\033[91m██\033[0m",
                  Flag.WALL: "██",
                  Flag.ENTRY: "\033[36m██\033[0m",
                  Flag.EXIT: "\033[35m██\033[0m",
                  }
        str_maze = []
        for row in pixels:
            str_row = [colors[p.read()] for p in row]
            str_maze.append(str_row)
        return "\n".join("".join(x) for x in str_maze)

    def _lock_pattern(self) -> None:
        """Close and lock all the cells that need to draw the pattern."""
        pattern = self._pattern

        if not pattern:
            return

        pattern_height = len(pattern)
        pattern_width = max(col for row in pattern for col in row) + 1

        if (self._width < pattern_width + 2) \
                or (self._height < pattern_height + 1):
            print("Maze is too small for this pattern")
            return

        start_col = ceil((self._width - pattern_width) / 2)
        start_row = ceil((self._height - pattern_height) / 2)

        for row_idx, row in enumerate(pattern):
            for col in row:
                cell = self._cells[start_row + row_idx][start_col + col]
                cell._walls = 15
                cell._locked = True
                cell._flag = Flag.PATTERN

    @staticmethod
    def _get_maze_hex(cells: list[list[Cell]]) -> str:
        """
        Converts the 2D list of cells into a representation of the walls
        in hex-value
        """
        hex_val = "0123456789abcdef"
        maze_hex = ""
        for cell_row in cells:
            for cell in cell_row:
                maze_hex += hex_val[cell._walls]
            maze_hex += "\n"
        return maze_hex

    def _export_maze(self, entry: tuple[int, int], exit: tuple[int, int],
                     solution: str) -> None:
        """Exports the maze into an output_maze.txt

        It shows the maze in hex form, entry, exit position and if solution
        the solution string 'NSEEWNS'
        """
        hex_maze = self._get_maze_hex(self._cells)

        with open("output_maze.txt", "w") as f:
            f.write(f"{hex_maze}\n")
            f.write(f"{entry[0]},{entry[1]}\n")
            f.write(f"{exit[0]},{exit[1]}\n")

            if solution:
                f.write(f"{solution}")

    def _get_pixel(self, pos: tuple[int, int]) -> Flag:
        """
        Pos is given as position in the grid, not the cell
        """
        row, col = pos
        wall_row = not row % 2
        wall_col = not col % 2
        px: Pixel = Pixel()
        if wall_row and wall_col:
            # Pixel is untouchable
            return Flag.WALL
        elif (not wall_row) and (not wall_col):
            # Pixel is a cell
            px.add_flag(self.cells[row // 2][col // 2].flag)
        elif wall_row and (not wall_col):
            # Pixel is a wall with cells top and bottom
            px.add_flag(self.cells[(row - 1) // 2][col].flag)
            px.add_flag(self.cells[(row + 1) // 2][col].flag)
        else:
            # Pixel is a wall with cells left and right
            px.add_flag(self.cells[row][(col - 1) // 2].flag)
            px.add_flag(self.cells[row][(col + 1) // 2].flag)
        return px.read()

    class Patterns():
        """This inner-class contains the available patterns that can be drawn
            inside the maze
        """
        NO_PATTERN: Optional[list[list[int]]] = None
        FORTY_TWO: list[list[int]] = [[0, 2, 4, 5, 6],
                                      [0, 2, 6],
                                      [0, 1, 2, 4, 5, 6],
                                      [2, 4],
                                      [2, 4, 5, 6]]
        SQUARE: list[list[int]] = [[0, 1, 2, 3, 4, 5, 6],
                                   [0, 1, 2, 3, 4, 5, 6],
                                   [0, 1, 2, 3, 4, 5, 6],
                                   [0, 1, 2, 3, 4, 5, 6],
                                   [0, 1, 2, 3, 4, 5, 6],
                                   [0, 1, 2, 3, 4, 5, 6],
                                   [0, 1, 2, 3, 4, 5, 6]]
        STAR: list[list[int]] = [[5],
                                 [4, 5, 6],
                                 [3, 4, 5, 6, 7],
                                 [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
                                 [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10],
                                 [1, 2, 3, 4, 5, 6, 7, 8, 9],
                                 [2, 3, 4, 5, 6, 7, 8],
                                 [2, 3, 4, 5, 6, 7, 8],
                                 [1, 2, 3, 4, 5, 6, 7, 8, 9],
                                 [1, 2, 3, 7, 8, 9],
                                 [2, 8],
                                 []]

        _lookup: dict[str, list[list[int]]] = {
            "42": FORTY_TWO,
            "SQUARE": SQUARE,
            "STAR": STAR
        }

        @classmethod
        def get(cls, name: Optional[str]) -> Optional[list[list[int]]]:
            """
            Gets the name of the desired pattern and returns the
            pattern config
            """
            if not name:
                return cls.NO_PATTERN
            return cls._lookup[name.upper()]
