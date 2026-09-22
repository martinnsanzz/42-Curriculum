# Built-in modules
from typing import Any, Self, Optional
from random import choice

# Local modules
from pydantic import BaseModel, model_validator


class MazeConfig(BaseModel):
    """Validated configuration model for maze generation and I/O.

    Parses and validates maze parameters loaded from ``config.txt``,
    converting comma-separated entry/exit strings into coordinate
    tuples and enforcing bounds/format constraints before the model
    is considered valid.

    Attributes:
        width: Maze width in cells.
        height: Maze height in cells.
        entry: Entry coordinate as ``(x, y)``, parsed from a
            comma-separated string by ``convert_tuple``.
        exit: Exit coordinate as ``(x, y)``, parsed the same way.
        output_file: Path to write generated maze output to.
        perfect: Whether the maze should be a "perfect" maze (no loops).
        seed: Optional RNG seed for reproducible generation.
        algorithm: Optional algorithm selector.
        display_mode: Optional display mode selector.
    """
    width: int
    height: int
    entry: tuple[int, int]
    exit: tuple[int, int]
    output_file: str
    perfect: bool
    build_anim: bool
    build_algorithm: str
    solve_algorithm: str
    seed: Optional[int]
    pattern: Optional[str]

    @model_validator(mode='after')
    def validate_entry_exit(self) -> Self:
        """Validate entry/exit valuess.

        Checks that entry and exit coordinates are non-negative,
        distinct from each other, and within the maze's width/height
        bounds.

        Returns:
            The validated ``MazeConfig`` instance, unchanged.

        Raises:
            CustomError: If entry or exit contains a negative value,
                if entry equals exit, if entry or exit falls outside
                the maze bounds.
        """
        for val in self.entry:
            if val < 0:
                raise ValueError(f"Entry can't be negative: {self.entry}")

        for val in self.exit:
            if val < 0:
                raise ValueError(f"Exit can't be negative: {self.exit}")

        if self.entry == self.exit:
            raise ValueError("Entry and exit can't be the same.")

        if self.entry[0] > self.width or self.entry[1] > self.height:
            raise ValueError("Entry point must be within maze bounds:"
                             f"{self.entry}")

        if self.exit[0] >= self.width or self.exit[1] >= self.height:
            raise ValueError("Exit point must be within maze bounds:"
                             f"{self.exit}")

        return self

    @model_validator(mode='after')
    def validate_maze_dimensions(self) -> Self:
        """Validate maze dimensions."""
        if self.width < 2 or self.height < 2:
            raise ValueError("Minimum size of maze 2x2. "
                             "Update width x height.")

        return self

    @model_validator(mode="after")
    def validate_build_algorithm(self) -> Self:
        accepted_algorithms = ["kruskal", "dfs"]

        if self.build_algorithm.lower() not in accepted_algorithms:
            raise ValueError("Use a valid build algorithm name.")
        self.build_algorithm = self.build_algorithm.lower()
        return self

    @model_validator(mode="after")
    def validate_maze_pattern(self) -> Self:
        accepted_patterns = ["42", "SQUARE", "STAR"]

        if self.pattern and self.pattern.upper() not in accepted_patterns:
            raise ValueError("Use a valid pattern name.")
        if self.pattern:
            self.pattern = self.pattern.upper()
        return self

    @model_validator(mode="after")
    def validate_solve_algorithm(self) -> Self:
        accepted_algorithms = ["shrink", "find", "search"]
        self.solve_algorithm = self.solve_algorithm.lower()

        if self.solve_algorithm not in accepted_algorithms:
            raise ValueError("Use a valid solve algorithm name")
        if not self.perfect and self.solve_algorithm == "shrink":
            raise ValueError("Can't use 'shrink' with imperfect maze")
        return self


def _config_parser(file_name: str) -> dict[str, Any]:
    """Read a config file and extract its key-value pairs.

    Args:
        file_name (str): Files name to read from.
    Returns:
        A mapping of uppercase config keys to their raw string values.
    """
    with open(file_name, "r") as file:
        content = file.readlines()
        return _extract_config_data(content)


def _extract_config_data(file_data: list[str]) -> dict[str, Any]:
    """Parse config file lines into a key-value mapping.

    Skips lines containing ``#`` (comments) and splits remaining
    lines on ``=`` into uppercase keys and stripped string values.

    Args:
        file_data (list[str]): Lines read from a config file.

    Returns:
        A mapping of uppercase config keys to their string values.
    """
    maze_config: dict[str, Any] = {}

    for line in file_data:
        if "#" not in line:
            tmp: list[str] = line.split("=")
            maze_config.update({tmp[0].upper(): (tmp[1].strip("\n"))})
    return maze_config


def load_maze_config() -> MazeConfig:
    """Load and validate the maze configuration from ``config.txt``.

    Reads ``config.txt`` from the current working directory and
    constructs a validated ``MazeConfig`` instance from its contents.

    Returns:
        A validated ``MazeConfig`` instance.

    Note:
        If config file does not contain key:value pair it provides a
        defualt value
    """
    config_dict = _config_parser("config.txt")
    build_algorithms = ["kruskal", "dfs"]
    solve_algorithms = ["shrink", "find", "search"]

    maze_config: MazeConfig = MazeConfig(
        width=config_dict.get("WIDTH", 20),
        height=config_dict.get("HEIGHT", 20),
        entry=tuple(config_dict.get("ENTRY", "0,0").split(",")),
        exit=tuple(config_dict.get("EXIT", "20,20").split(",")),
        output_file=config_dict.get("OUTPUT_FILE", "maze.txt"),
        perfect=config_dict.get("PERFECT", False),
        seed=config_dict.get("SEED", None),
        build_algorithm=config_dict.get("BUILD_ALGORITHM",
                                        choice(build_algorithms)),
        solve_algorithm=config_dict.get("SOLVE_ALGORITHM",
                                        choice(solve_algorithms)),
        build_anim=config_dict.get("BUILD_ANIM", False),
        pattern=config_dict.get("MAZE_PATTERN", None)
    )
    return maze_config
