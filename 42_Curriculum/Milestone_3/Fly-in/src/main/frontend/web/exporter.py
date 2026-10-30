# Built-in modules
from pathlib import Path
from typing import Any
import shutil
from json import dump


class MapExporter:
    """Write the parsed maps, their simulations and an index as JSON files.

    The files are written into `output_dir`, grouped by difficulty (the
    name of the source map's parent folder):

        <output_dir>/<difficulty>/<map stem>.json
        <output_dir>/<difficulty>/<map stem>_sim.json
        <output_dir>/index.json

    Attributes:
        output_dir: Directory that receives all the generated files.
    """
    OUTPUT_DIR = Path(__file__).resolve().parent / "static" / "parsed_maps"
    INDENT = 4

    def __init__(self, output_dir: Path = OUTPUT_DIR) -> None:
        """Store the output directory. Nothing is written yet.

        Args:
            output_dir: Directory to write into. Defaults to
                `static/parsed_maps` next to this file.
        """
        self.output_dir = output_dir

    def prepare(self, map_paths: list[Path]) -> None:
        """Reset the output directory and create one subfolder per difficulty.

        Delete `output_dir` and all its contents, ignoring errors, then
        create a subdirectory for each map, named after the map's parent
        directory.

        Args:
            map_paths: Paths of the source map files.
        """
        shutil.rmtree(self.output_dir, ignore_errors=True)

        for path in map_paths:
            (self.output_dir / path.parent.name).mkdir(parents=True,
                                                       exist_ok=True)

    def write_map(self, map_path: Path, data: dict[str, Any]) -> str:
        """Write the parsed data of a map to a JSON file.

        Args:
            map_path: Path of the source map file. Its parent directory
                name and file stem decide where the file goes.
            data: JSON-serializable dictionary to write.

        Returns:
            The path of the written file, relative to the parent of
            `output_dir`, using forward slashes (the form the browser
            fetches).
        """
        target = self._target(map_path, ".json")
        self._write_json(target, data)
        return self._relative(target)

    def write_simulation(self, map_path: Path,
                         simulation: dict[str, Any]) -> str:
        """Write the simulation of a map next to its map file.

        Args:
            map_path: Path of the source map file. The simulation file
                is named "<map stem>_sim.json".
            simulation: JSON-serializable dictionary to write.

        Returns:
            The path of the written file, in the same form as
            `write_map`.
        """
        target = self._target(map_path, "_sim.json")
        self._write_json(target, simulation)
        return self._relative(target)

    def write_index(self, entries: list[dict[str, str]]) -> None:
        """Write an index of all parsed maps to `index.json`.

        Args:
            entries: One dictionary per map, with its name, difficulty,
                map path and simulation path.
        """
        self._write_json(self.output_dir / "index.json", entries)

    def _target(self, map_path: Path, suffix: str) -> Path:
        """Return the output file for a map: <difficulty>/<stem><suffix>."""
        return (self.output_dir / map_path.parent.name
                / f"{map_path.stem}{suffix}")

    def _relative(self, target: Path) -> str:
        """Return a path relative to the parent of `output_dir`."""
        return target.relative_to(self.output_dir.parent).as_posix()

    def _write_json(self, target: Path, data: Any) -> None:
        """Write data to a JSON file, creating its folder if needed."""
        target.parent.mkdir(parents=True, exist_ok=True)
        with open(target, "w", encoding="utf-8") as f:
            dump(data, f, indent=self.INDENT)
