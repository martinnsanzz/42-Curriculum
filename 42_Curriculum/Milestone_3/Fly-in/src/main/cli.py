# Built-in Modules
from argparse import Namespace, ArgumentParser, ArgumentError
from pathlib import Path
from typing import Optional

# Local modules
from .parser import parse_map
from .map_selection import choose_map
from .frontend import TerminalPrinter
from .exceptions import FlyInError
from .backend import Graph, PathFinder, Simulation

ERROR = "------ERROR FOUND------"

def main() -> int:
    try:
        args = parse_args()

        if args.visual:
            ...
        map_path = resolve_map(args)
        if map_path is None:
            return 0

        graph = Graph.from_drone_map(parse_map(map_path))
        simulation = Simulation(graph, PathFinder(graph))
        simulation.run(TerminalPrinter.print_step)

    except (FlyInError, ArgumentError) as e:
        TerminalPrinter.print_error(f"{ERROR}\n{e}")
        return 1
    return 0

def resolve_map(args: Namespace) -> Optional[Path]:
    if args.map is not None:
        return Path(args.map)
    return choose_map()

def parse_args() -> Namespace:
    parser = ArgumentParser(exit_on_error=False)
    parser.add_argument("--visual", action="store_true",
                        help="show the visual display")
    parser.add_argument("--map",
                        type=Path,
                        required=False,
                        metavar="PATH_TO_MAP",
                        help="map file to load (otherwise choose from a menu)")

    try:
        known, unknown = parser.parse_known_args()
    except ArgumentError as e:
        msg = str(e)
        raise ArgumentError(None, msg.capitalize()) from e

    if unknown:
        raise ArgumentError(None,
                    "Incorrect argument on command line:\n  - "
                    "Correct usage: uv run python -m src\n")
    return known