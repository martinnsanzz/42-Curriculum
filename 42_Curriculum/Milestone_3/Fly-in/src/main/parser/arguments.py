# Built-in Modules
from argparse import Namespace, ArgumentParser, ArgumentError
from pathlib import Path

def parse_args() -> Namespace:
    """Parse the command line arguments.
    
    It takes 2 flags:
        --visual: bool -> If false it displays the result of the simulation
                          on the terminal, else on the visualizer.
        --map: Path -> A path to the map the simulation needs to run on.
                       This is an optional flag, since there's an interactive
                       map selector with curses. This is use for testing
                       purposes.
    
    Notes:
        If `--visual` flag is True `--map` can't be present
    """
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
                    "Correct usage: uv run python -m src")
    elif known.visual and known.map:
        raise ArgumentError(None,
                    "If `--visual` flag is present `--map` shouldn't"
                    "be present on the CLI.")
    return known