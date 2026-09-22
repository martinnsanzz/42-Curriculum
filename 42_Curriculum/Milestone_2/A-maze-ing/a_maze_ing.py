#!/usr/bin/env python3

# Built-in modules
import sys
import curses
from pydantic import ValidationError

# Local modules
from frontend.tui_logic.state_loop import main as ui_main


def main() -> None:
    """Initializes curses and starts the main program."""
    curses.wrapper(ui_main)


if __name__ == "__main__":
    try:
        _, config_file = sys.argv
        if config_file != "config.txt":
            raise Exception("Incorrect argument. Argument "
                            "can only be 'config.txt' "
                            "(e.g., ./a_maze_ing.py 'context.txt')")
    except ValueError:
        print("\033[91mIncorrent command: Too many arguments, only "
              "'config.txt' is accepted !\033[0m")

    try:
        main()
    except ValidationError as e:
        print(f"\033[91m{str(e.errors()[0]['msg'])}\033[0m")
    except BaseException as e:
        print(f"\033[91m{str(e)}\033[0m")
