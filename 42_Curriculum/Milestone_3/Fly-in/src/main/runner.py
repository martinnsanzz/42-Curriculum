# Built-in Modules
from argparse import Namespace
from pathlib import Path

# Local modules
from .parser import parse_map
from .exceptions import MapError
from .map_selection import resolve_map, read_map_folder
from .frontend import TerminalPrinter, VisualizerServer, MapExporter, \
                      serialize_map
from .backend import Graph, PathFinder, Simulation


def run_terminal(args: Namespace) -> None:
    """Run the drone simulation and print each step to the terminal.

    Resolve the map file from the CLI arguments, parse it into a graph,
    compute the drone paths, and print the resulting simulation steps.

    Args:
        args (Namespace): Parsed command-line arguments, used by
            `resolve_map` to locate the map file. [INFERRED]
    """
    map_path = resolve_map(args)
    if map_path is None:
        return

    graph = Graph.from_drone_map(parse_map(map_path))
    simulation = Simulation(graph, PathFinder(graph))
    steps = simulation.run()
    TerminalPrinter.output_simulation(steps)


def run_visual() -> None:
    """Simulate every map and serve the results through an HTTP server.

    Discover the maps in the map folder, parse and serialize each one,
    run the simulation on its graph, and write the map and simulation
    JSON files. Then write an index of all processed maps and start the
    HTTP server.

    Raises:
        MapError: If a map fails to parse. The message is prefixed with
            the name of the offending map file.
    """
    map_paths: list[Path] = []
    entries: list[dict[str, str]] = []
    exporter = MapExporter()

    for key, value in read_map_folder().items():
        for val in value:
            map_paths.append(Path(key + "/" + val))

    exporter.prepare(map_paths)

    for map_path in map_paths:
        try:
            drone_map = parse_map(map_path)
        except MapError as e:
            raise MapError(f"Map: {map_path.name}\n"
                           f"   {str(e)}")
        data = serialize_map(drone_map)

        graph = Graph.from_drone_map(parse_map(map_path))
        simulation = Simulation(graph,
                                PathFinder(graph)).get_simulation(map_path)

        rel = exporter.write_map(map_path, data)
        sim_rel = exporter.write_simulation(map_path, simulation)

        entries.append({"name": map_path.stem,
                        "difficulty": map_path.parent.name,
                        "path": rel,
                        "simulation": sim_rel})
    exporter.write_index(entries)

    try:
        VisualizerServer().run()
    except OSError as e:
        TerminalPrinter.print_error(str(e) + "\n")
