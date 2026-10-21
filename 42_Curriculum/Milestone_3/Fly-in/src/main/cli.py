# Built-in Modules
from argparse import ArgumentError

# Local modules
from .frontend import TerminalPrinter
from .exceptions import FlyInError
from .runner import run_terminal, run_visual
from .parser import parse_args


def main() -> int:
    """Starting point of the program.

    Based on the CLI it will display the simulation in the terminal
    or the visualizer.
    """
    try:
        args = parse_args()

        if args.visual:
            return run_visual()
        return run_terminal(args)
    except (FlyInError, ArgumentError) as e:
        TerminalPrinter.print_error(f"{e}\n")
        return 1
