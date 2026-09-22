# Built-in modules
import curses
from typing import Any

# Local modules
from .windows_config import WINDOWS


class Window():
    """A class representing a terminal menu interface.

    This class handles the rendering of a window with a title and
    selectable options within a cuses environment.

    Attributes:
        window (curses.window): The main terminal window object.
        title (str): The title displayed at the top of the menu.
        options (list): A list of dictionaries containing option labels
                        and positions.
    """

    def __init__(self, stdscr: curses.window, config: dict[str, Any]) -> None:
        """Initializes the Window with a terminal window and config.

        Args:
            stdscr (curses.window): The standard curses window object.
            config (dict[str, Any]): A dictionary containing menu config,
                including 'pos', 'options', 'title' etc...
        """
        self.stdscr = stdscr
        self.title = config["title"]
        self.options: list[dict[str, Any]] = config.get("options", [])

    def draw_win(self, config: dict[str, Any]) -> curses.window:
        """Draws the menu window at a calculated position

        Calculates the coords based on the provided position config
        (center or top-maze), creates a new sub-window, draws a box if
        desired and renders the title and options.

        Args:
            config (dict[str, Any]): A dictionary containing menu config,
                including 'pos', 'options', 'title' etc...

        Returns:
            curses.window: The newly created and rendered menu window object
        """
        maze_config = WINDOWS["maze_window"]["sub_maze"]
        term_h, term_w = self.stdscr.getmaxyx()

        pos = config["pos"]

        if pos[0] == "center":
            y = (term_h - config["h"]) // 2
            x = (term_w - config["w"]) // 2
        elif pos[0] == "top-maze":
            y = ((term_h - maze_config["h"]) // 2) - config["h"]
            x = (term_w - config["w"]) // 2

        menu = curses.newwin(config["h"], config["w"], y, x)

        if not self.title == "Maze":
            menu.box()
            menu.addstr(0, 0, self.title, curses.A_BOLD)

        for _ in self.options:
            for key, value in _.items():
                menu.addstr(value[0], value[1], key)

        return menu


class MazeWindow:
    """A class that representing the maze window.

    This class handles the rendering of the window containing the maze and
    the options window on top of the maze.

    Attributes:
        stdscr (curses.window): The main terminal window object.
        opt_win (curses.window): The window displaying the options
            for the user.
        maze_win (curses.window): The window displaying the maze.
    """

    def __init__(self, stdscr: curses.window) -> None:
        """Initializes and builds the maze and options windows to the terminal.

        Args:
            stdscr (curses.window): The standard curses window object.
        """
        self.stdscr = stdscr
        self.opt_win: curses.window
        self.maze_win: curses.window
        self._build_windows()

    def _build_windows(self) -> None:
        """Initializes and draws the maze and options windows to the terminal.

        Retrieves configuration for both the options and maze sub-windows,
        instantiates Window objects, and renders them. Validates that the
        configured dimensions fit within the current terminal screen size.

        Raises:
            RuntimeError: If the options menu dimensions exceed the screen
                height, or if the maze dimensions exceed the screen width.
                The error message advises reducing the respective
                dimensions in 'config.txt'.
        """
        config_opt = WINDOWS["maze_window"]["sub_options"]
        opt = Window(self.stdscr, config_opt)
        try:
            self.opt_win = opt.draw_win(config_opt)
        except BaseException:
            raise RuntimeError("Reduce height in 'config.txt'")

        config_maze = WINDOWS["maze_window"]["sub_maze"]
        maze_menu = Window(self.stdscr, config_maze)
        try:
            self.maze_win = maze_menu.draw_win(config_maze)
        except BaseException:
            raise RuntimeError("Reduce width in 'config.txt'")

    def refresh_all(self) -> None:
        """Refreshes both windows to display updated content"""
        assert self.maze_win is not None
        assert self.opt_win is not None
        self.maze_win.refresh()
        self.opt_win.refresh()
