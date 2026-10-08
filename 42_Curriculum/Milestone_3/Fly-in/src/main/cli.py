# Built-in Modules
from argparse import Namespace, ArgumentParser, ArgumentError
from pathlib import Path

# Local modules
from .backend import parse_map
from .colors import print_error
from .exceptions import FlyInError

ERROR = "------ERROR FOUND------"

def main() -> int:
    try:
        args = parse_args()
        fly_map = parse_map(args.map)
        # print(fly_map)
    except (FlyInError, ArgumentError) as e:
        print_error(f"{ERROR}\n{e}")
        return 1
    return 0

def parse_args() -> Namespace:
    parser = ArgumentParser(exit_on_error=False)
    parser.add_argument("--map",
                        type=Path,
                        required=True,
                        metavar="PATH_TO_MAP")
    
    try:
        known, unknown = parser.parse_known_args()
    except ArgumentError as e:
        msg = str(e)
        raise ArgumentError(None, msg.capitalize()) from e

    if unknown:
        raise ArgumentError(None,
                    "Incorrect argument on command line:\n  - "
                    "Correct usage: src --map "
                    "<PATH_TO_MAP>")
    return known