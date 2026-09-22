# Built-in modules
import curses

# Local modules
from ..window_class import Window
from ..windows_config import WINDOWS
from ..state_class import State
from .maze_loop import maze_loop


def start_window(stdscr: curses.window) -> State:
    """Display the start window and collect the user's menu selection.

    Draws the start-screen window, then blocks on key input until a
    valid ``State`` member is returned by ``handle_input``.

    Args:
        stdscr: The curses standard screen object.

    Returns:
        The ``State`` enum member selected by the user.
    """
    config = WINDOWS["start_window"]
    start_win = Window(stdscr, config)
    menu_window = start_win.draw_win(config)

    action = None
    start_actions = (State.QUIT,
                     State.GEN_MAZE)
    while action not in start_actions:
        action = State.handle_input(menu_window.getkey(), State.START_WIN)

    menu_window.refresh()
    return action


def main(stdscr: curses.window) -> None:
    """Run the maze application's main state-machine loop.

    Dispatches control to the appropriate handler for each state
    (start window, maze generation/play) until ``State.QUIT`` is
    reached. Tracks ``current_color`` across states so the active
    color scheme persists between window transitions.

    Args:
        stdscr: The curses standard screen object.
    """
    col_idx = 0
    col_list = (
        curses.COLOR_WHITE,
        curses.COLOR_BLACK,
        curses.COLOR_CYAN,
        curses.COLOR_RED,
        curses.COLOR_YELLOW,
        curses.COLOR_MAGENTA,
        curses.COLOR_GREEN,
        curses.COLOR_BLUE,
    )
    for idx, color in enumerate(col_list):
        curses.init_pair(idx + 1, color, curses.COLOR_BLACK)

    # Hide the cursor
    curses.curs_set(0)
    cur_state = State.START_WIN

    # Initialise the starting state
    while cur_state is not State.QUIT:
        if cur_state is State.START_WIN:
            cur_state = start_window(stdscr)
        elif cur_state is State.GEN_MAZE:
            stdscr.clear()
            cur_state, col_tup = maze_loop(stdscr, (col_idx, len(col_list)))
            col_idx = col_tup[0]
