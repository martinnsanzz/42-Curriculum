# Built-in Modules
from argparse import Namespace

# Local modules
from .parser import parse_map
from .map_selection import resolve_map
from .frontend import TerminalPrinter, run_http_server
from .backend import Graph, PathFinder, Simulation


def run_terminal(args: Namespace) -> int:
    """Run the drone simulation and print each step to the terminal.

    Resolve the map file from the CLI arguments, parse it into a graph,
    compute the drone paths, and print the resulting simulation steps.

    Args:
        args (Namespace): Parsed command-line arguments, used by
            `resolve_map` to locate the map file. [INFERRED]

    Returns:
        int: Exit code. Always 0, including when no map path could be
            resolved which means the user quit the interactive curses
            menu.
    """
    map_path = resolve_map(args)
    if map_path is None:
        return 0

    graph = Graph.from_drone_map(parse_map(map_path))
    simulation = Simulation(graph, PathFinder(graph))
    steps = simulation.run()
    TerminalPrinter.output_simulation(steps)
    return 0


def run_visual() -> int:
    """Run the simulation and display the results in a different window"""
    run_http_server()
    return 0
