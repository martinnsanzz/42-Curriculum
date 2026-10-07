# Built-in modules
from pathlib import Path

# Local modules
from ..exceptions import MapError


def load_map(map_path: Path) -> str:
    try:
        with open(map_path, "r") as f:
            content = f.read()
    except OSError as e:
        raise MapError(f"{e.strerror} '{map_path}'") from e
    except UnicodeDecodeError as e:
        raise MapError(f"Not a valid text file '{map_path}") from e

    if not content.strip():
        raise MapError(f"File is empty '{map_path}")
    return content