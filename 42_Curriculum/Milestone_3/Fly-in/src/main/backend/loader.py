# Built-in modules
from pathlib import Path

# Local modules
from .exceptions import MapError


def load_map(map_path: Path) -> str:
    try:
        with open(map_path, "r") as f:
            content = f.read()
    except OSError as e:
        raise MapError(f"{map_path}: {e.strerror}") from e
    except UnicodeDecodeError as e:
        raise MapError(f"{map_path}: not a valid text file") from e

    if not content.strip():
        raise MapError(f"{map_path}: file is empty")
    return content