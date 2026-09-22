# Built-int modules
from typing import Any
from pydantic import ValidationError

# Local modules
from .config_parser import load_maze_config

try:
    maze_config = load_maze_config()
except ValidationError as e:
    print(f"\033[91m{str(e.errors()[0]['msg'])}\033[0m")
    quit()
except ValueError as e:
    print(f"\033[91m{str(e)}\033[0m")
    quit()

# Window configuration map. Each key is a unique window state.
WINDOWS: dict[str, Any] = {
    "start_window": {
        "title": "A-maze-ing",
        "options": [{"(G) Generate maze": [3, 4],
                    "(Q) Quit": [5, 4]}],
        "h": 10, "w": 25,
        "pos": ["center"],
        "keys": {"G": "maze_window", "Q": "quit"}
    },
    "maze_window": {
        "sub_options": {
            "title": "Maze-menu",
            "options": [{"(R) Regenerate maze": [2, 3],
                         "(C) Change color": [3, 3],
                         "(Q) Quit": [3, 25],
                         "(S) Solve": [2, 25]}],
            "h": 6,
            "w": 50,
            "pos": ["top-maze"],
            "keys": {"R": "regen", "C": "change_color", "Q": "quit"}
        },
        "sub_maze": {
            "title": "Maze",
            "options": [],
            "h": 2 * maze_config.height + 1,
            "w": 2 * (2 * maze_config.width + 1) + 1,
            "pos": ["center"],
        }
    }
}
