# Built-in modules
from pathlib import Path
from typing import Any
import shutil
from json import dump

# Local modules

PARSED_MAPS_DIR = Path("src/main/frontend/web/static/parsed_maps/")

def prepare_output_dirs(map_paths: list[Path]) -> None:
    """Reset the output directory and create one subfolder per difficulty.

    Delete ``PARSED_MAPS_DIR`` and all its contents, ignoring errors,
    then create a subdirectory for each map, named after the map's
    parent directory.
    """
    shutil.rmtree(PARSED_MAPS_DIR, ignore_errors=True)

    for path in map_paths:
        (PARSED_MAPS_DIR / path.parent.name).mkdir(parents=True,
                                                   exist_ok=True)

def write_map(map_path: Path, data: dict[str, Any]) -> str:
    """Write the parsed data of a map to a JSON file.

    The file is written to
    ``PARSED_MAPS_DIR/<parent dir name>/<map file stem>.json`` with a
    4-space indent. The target directory must already exist (see
    ``prepare_output_dirs``).

    Returns:
        The path of the written file, relative to the parent of
        ``PARSED_MAPS_DIR``, using forward slashes.
    """
    target = (PARSED_MAPS_DIR / map_path.parent.name
              / f"{map_path.stem}.json")
    with open(target, "w", encoding="utf-8") as f:
        dump(data, f, indent=4)
    return target.relative_to(PARSED_MAPS_DIR.parent).as_posix()

def write_simulation(sim_path: Path, simulation: dict[str, Any]) -> None:
    """Write the simulation of a map to a JSON file."""
    target = (PARSED_MAPS_DIR / sim_path.parent.name
              / f"{sim_path.stem}.json")
    with open(target, "w", encoding="utf-8") as f:
        dump(simulation, f, indent=4)

def write_index(entries: list[dict[str, str]]) -> None:
    """Write an index of all parsed maps to ``index.json``.

    The file is written to ``PARSED_MAPS_DIR/index.json`` with a
    4-space indent. Each entry describes one map: its name, its
    difficulty and the relative path of its JSON file.
    """
    target = (PARSED_MAPS_DIR / "index.json")
    with open(target, "w", encoding="utf-8") as f:
        dump(entries, f, indent=4)