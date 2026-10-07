# Built-in Modules
from argparse import Namespace, ArgumentParser, ArgumentError
from pathlib import Path
from sys import stderr

# Local modules
from .backend import load_map
from .colors import print_error
from .exceptions import FlyInError


def _main() -> int:
    try:
        args = parse_args()
        fly_map = load_map(args.map)
    except (FlyInError, ArgumentError) as e:
        print_error(f"Error: {e}")
        return 1
    return 0

def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument("--map",
                        type=Path,
                        metavar="PATH_TO_MAP")
    known, unknown = parser.parse_known_args()

    if unknown:
        raise ArgumentError(None,
                    "Incorrect argument on command line:\n  - "
                    "Correct usage: src --map "
                    "<PATH_TO_MAP>")
    return known