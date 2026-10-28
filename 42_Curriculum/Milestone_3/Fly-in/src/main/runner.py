# Built-in Modules
from argparse import Namespace
from pathlib import Path

# Local modules
from .parser import parse_map
from .exceptions import MapError
from .map_selection import resolve_map, read_map_folder
from .frontend import TerminalPrinter, run_http_server, prepare_output_dirs, \
                      serialize_map, write_map, write_simulation, write_index
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
    map_paths: list[Path] = []
    entries: list[dict[str, str]] = []

    for key, value in read_map_folder().items():
        for val in value:
            map_paths.append(Path(key + "/" + val))

    prepare_output_dirs(map_paths)

    for map_path in map_paths:
        try:
            drone_map = parse_map(map_path)
        except MapError as e:
            raise MapError(f"Map: {map_path.name}\n"
                           f"   {str(e)}")
        data = serialize_map(drone_map)

        graph = Graph.from_drone_map(parse_map(map_path))
        simulation = Simulation(graph, PathFinder(graph)).get_simulation(map_path)

        rel = write_map(map_path, data)
        sim_path = rel.rstrip(".json") + "_sim.json"
        write_simulation(Path(sim_path), simulation)

        entries.append({"name": map_path.stem,
                        "difficulty": map_path.parent.name,
                        "path": rel,
                        "simulation": sim_path})
    write_index(entries)
    # run_http_server()
    return 0