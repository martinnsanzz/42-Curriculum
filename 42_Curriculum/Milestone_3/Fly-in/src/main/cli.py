# Built-in Modules
from argparse import Namespace, ArgumentParser, ArgumentError
from pathlib import Path

# Local modules
from .parser import parse_map
from .colors import print_error
from .exceptions import FlyInError
from .backend import Graph

ERROR = "------ERROR FOUND------"

def main() -> int:
    try:
        args = parse_args()
        drone_map = parse_map(args.map)
        graph = Graph.from_drone_map(drone_map)

        print(bfs(graph, "start"))
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