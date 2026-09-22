# Built-in modules
import curses
import time

# Local modules
from src.mazegen.classes import Flag, Pixel


def draw_maze(maze_window: curses.window, maze_grid: list[list[Pixel]],
              colors: tuple[int, int], solved: bool = False) -> None:
    """Draw the maze using curses"""
    flag: Flag
    col_idx, col_count = colors
    for row in range(len(maze_grid)):
        for col in range(len(maze_grid[row])):
            flag = maze_grid[row][col].read()
            if not solved and flag is Flag.SOLUTION:
                flag = Flag.EMPTY
            try:
                cell_color = ((flag.value + col_idx - 1) % col_count) + 1
                maze_window.addstr(row, col * 2, "██",
                                   curses.color_pair(cell_color))
            except IndexError:
                raise IndexError(f"{flag} - {flag.value} - {col_idx} = ERROR")
    maze_window.refresh()


def draw_solution(maze_window: curses.window, steps: list[tuple[int, int]],
                  colors: tuple[int, int]) -> None:
    """Draw the solution steps using curses"""
    col_idx, col_count = colors
    flag: Flag
    i: int = 0
    last_step: int = len(steps) - 1
    for row, col in steps:
        if i == 0:
            flag = Flag.ENTRY
        elif i == last_step:
            flag = Flag.EXIT
        else:
            flag = Flag.SOLUTION
        cell_color = (flag.value + col_idx) % col_count
        maze_window.addstr(row, col * 2, "██", curses.color_pair(cell_color))
        maze_window.refresh()
        time.sleep(0.01)
        i += 1
