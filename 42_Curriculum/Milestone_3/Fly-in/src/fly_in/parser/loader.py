# Built-in modules
from pathlib import Path
from typing import NamedTuple

# Local modules
from fly_in.exceptions import MapError

ALLOWED_KEYS = ["nb_drones", "start_hub", "end_hub", "hub", "connection"]

class RawLine(NamedTuple):
    line: int
    key: str
    value: str

def load_map(map_path: Path) -> list[RawLine]:
    raw_map: list[RawLine] = []

    try:
        with open(map_path, "r", encoding="utf-8") as f:
            content = f.readlines()
    except OSError as e:
        raise MapError(f"{e.strerror} '{map_path}'") from e
    except UnicodeDecodeError as e:
        raise MapError(f"Not a valid text file '{map_path}'") from e

    for i, line in enumerate(content):
        if not line.startswith("#") and not line.strip() == "":
            if ": " not in line:
                 raise MapError("Incorrect line format:\n" \
                                "   - Correct -> 'name': 'parameters'\n" \
                                f"Line to fix -> {line}\n")

            tmp = line.strip().split(": ")

            if len(tmp) == 1:
                raise MapError("Key must be followed by arguments:\n"
                               "   - Correct -> 'name': 'parameters'\n" \
                               f"Line to fix -> {line}\n")
            if tmp[0] not in ALLOWED_KEYS:
                 raise MapError(f"Incorrect key found in line '{i+1}':\n" \
                                f"  - Found key -> '{tmp[0]}'\n" \
                                f"Allowed -> {ALLOWED_KEYS}\n")
            raw_line = RawLine(line=i+1, key=tmp[0], value=tmp[1])
            raw_map.append(raw_line)

    if len(raw_map) == 0:
            raise MapError(f"File is empty '{map_path}\n")
    return raw_map