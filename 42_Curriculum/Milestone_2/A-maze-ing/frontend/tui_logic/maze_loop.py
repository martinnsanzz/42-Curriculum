# Built-in modules
import curses
from typing import Any, Callable

# Local modules
from src.mazegen import MazeGenerator
from .maze_display import draw_maze, draw_solution
from ..config_parser import load_maze_config, MazeConfig
from ..window_class import MazeWindow
from ..state_class import State
from ..windows_config import WINDOWS


def maze_loop(stdscr: curses.window, colors: tuple[int, int]) \
                -> tuple[State, tuple[int, int]]:
    """Run the maze display and interaction loop until exit or regen.

    Loads the maze configuration, builds the maze grid, and repeatedly
    draws and refreshes the maze window while dispatching user input
    to color-change, regenerate, or quit actions.

    Args:
        stdscr (curses.window): The curses standard screen object.
        current_color (int | None): The active color scheme index, or
        ``None`` if no color has been set yet (triggers
        ``change_color()``).

    Returns:
        A tuple of ``(action, current_color)`` where ``action`` is the
        resulting ``State`` (e.g. ``State.QUIT`` or the value from
        ``resize_maze``) and ``current_color`` is the color scheme
        index to carry into the next state.
    """
    conf = load_maze_config()
    maze_window_obj = MazeWindow(stdscr)

    stdscr.refresh()
    maze_win = maze_window_obj.maze_win

    solve_anim = make_anim_fun(maze_win, colors)
    maze_gen = MazeGenerator(conf.width, conf.height, conf.build_algorithm,
                             conf.solve_algorithm, conf.perfect, conf.seed,
                             conf.pattern, solve_anim)

    maze = maze_gen.generate(conf.build_anim)
    solution = ""
    solved = False
    action = None

    while True:
        curses.flushinp()
        draw_maze(maze_win, maze._get_maze(), colors, solved)
        maze._export_maze(conf.entry, conf.exit, solution)
        maze_window_obj.refresh_all()
        action = State.handle_input(stdscr.getkey(), action)

        if action == State.GEN_MAZE:
            action, conf = regen_maze(conf)
            return action, colors
        elif action == State.CHANGE_COLOR:
            colors = ((colors[0] % colors[1]) + 1, colors[1])
        elif action == State.QUIT:
            return State.QUIT, colors
        elif action is State.SOLVE:
            if not solved:
                maze_gen.set_animation(make_anim_fun(maze_win, colors))
                result = maze_gen.solve(maze, conf.entry, conf.exit)
                if result is None:
                    solved = False
                    continue
                solution, steps = result
                draw_solution(maze_win, steps, colors)
                solved = True
            else:
                solved = False


def regen_maze(old_config: MazeConfig) -> tuple[State, MazeConfig]:
    """Detect a config change and update window dimensions if needed.

    Reloads the maze configuration and compares it against the given
    one. If they differ, resizes the maze sub-window's height/width in
    ``WINDOWS`` to match the new dimensions.

    Args:
        old_config (MazeConfig): The maze configuration currently in use.

    Returns:
        A tuple of ``State.GEN_MAZE`` to generate a new maze and
        ``conf`` to get the new configuration of the maze
    """
    conf = load_maze_config()

    if (old_config != conf):
        WINDOWS["maze_window"]["sub_maze"]["h"] = 2 * conf.height + 1
        WINDOWS["maze_window"]["sub_maze"]["w"] = 2 * (2 * conf.width + 1) + 1
    return State.GEN_MAZE, conf


def make_anim_fun(maze_win: curses.window,
                  colors: tuple[int, int]) -> Callable[[Any, bool], None]:
    """Creates animation function"""
    def animation_expression(grid: Any, show_path: bool) -> None:
        return draw_maze(maze_win, grid, colors, show_path)
    return animation_expression

# def make_anim_fun(maze_win: curses.window,
#                   colors: tuple[int, int]) -> Callable[[Any, bool], None]:
#     """Creates animation function"""
#     def animation_expression(grid: Any, show_path: bool) -> None:
#         return draw_maze(maze_win, grid, colors, show_path)
#     return animation_expression
