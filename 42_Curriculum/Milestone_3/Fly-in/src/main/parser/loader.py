# Built-in modules
from pathlib import Path
from typing import NamedTuple

# Local modules
from ..exceptions import MapError

ALLOWED_KEYS = ["nb_drones", "start_hub", "end_hub", "hub", "connection"]

class RawLine(NamedTuple):
    """A non-comment, non-blank line of a map file, split into key and value.

    Attributes:
        line (int): 1-based line number in the source file.
        key (str): Setting name before the first `': '` separator. Always
            a member of `ALLOWED_KEYS`.
        value (str): Text following the `': '` separator. [INFERRED]
    """
    line: int
    key: str
    value: str

def load_map(map_path: Path) -> list[RawLine]:
    """Read a map file and return its settings as raw key/value lines.

    Skip comment lines (starting with `#`) and blank lines. Split every
    other line on `': '` and check that the key is allowed. No validation
    of the values is done here.

    Args:
        map_path (Path): Path to the map file. Read as UTF-8.

    Returns:
        list[RawLine]: One entry per setting line, in file order.

    Raises:
        MapError: If the file cannot be opened or read (the `OSError` is
            chained as the cause).
        MapError: If the file is not valid UTF-8 text (the
            `UnicodeDecodeError` is chained as the cause).
        MapError: If a line does not contain `': '`, or has no value after
            the key.
        MapError: If a key is not in `ALLOWED_KEYS`.
        MapError: If the file contains no setting lines.
    """
    raw_map: list[RawLine] = []

    try:
        with open(map_path, "r", encoding="utf-8") as f:
            content = f.readlines()
    except OSError as e:
        raise MapError(f"{e.strerror} '{map_path}'") from e
    except UnicodeDecodeError as e:
        raise MapError(f"Not a valid text file '{map_path}'") from e

    for i, line in enumerate(content):
        if not line.strip().startswith("#") and not line.strip() == "":
            stripped = line.strip()

            if stripped.endswith(":"):
                raise MapError("Key must be followed by arguments:\n"
                               "   - Correct -> 'name': 'parameters'\n"
                               f"Line {i + 1} to fix -> {stripped}")
            if ": " not in stripped:
                raise MapError("Incorrect line format:\n"
                               "   - Correct -> 'name': 'parameters'\n"
                               f"Line {i + 1} to fix -> {stripped}")

            key, value = stripped.split(": ", 1)
            value = value.strip()

            if key not in ALLOWED_KEYS:
                raise MapError(f"Incorrect key found in line '{i + 1}':\n"
                               f"  - Found key -> '{key}'\n"
                               f"Allowed -> {ALLOWED_KEYS}")
            raw_map.append(RawLine(line=i + 1, key=key, value=value))

    if len(raw_map) == 0:
            raise MapError(f"File is empty '{map_path}'")
    return raw_map