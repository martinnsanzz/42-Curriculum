# Built-in modules
from typing import Optional
from sys import stderr

# Local Modules
from ...backend import Move

RGB = tuple[int, int, int]

COLORS: dict[str, RGB] = {
        "black": (0, 0, 0), "blue": (0, 0, 255), "brown": (165, 42, 42),
        "crimson": (220, 20, 60), "cyan": (0, 255, 255),
        "darkred": (139, 0, 0), "gold": (255, 215, 0),
        "lime": (0, 255, 0), "magenta": (255, 0, 255),
        "maroon": (128, 0, 0), "orange": (255, 165, 0),
        "purple": (128, 0, 128), "red": (255, 50, 0),
        "violet": (238, 130, 238), "yellow": (255, 255, 0),
        "green": (0, 128, 0), "gray": (128, 128, 128),
        }

DEFAULT_ZONE_COLOR = (128, 128, 128)

RAINBOW: list[RGB] = [
    COLORS["red"], COLORS["orange"], COLORS["gold"],
    COLORS["green"], COLORS["cyan"], COLORS["blue"],
    COLORS["violet"]
]

RESET = "\033[0m"
BOLD = "\033[1m"

class TerminalPrinter:
    @classmethod
    def print_error(cls, text: str) -> None:
        if not stderr.isatty():
            print(text, file=stderr)
        red = cls.build_ansi(COLORS["red"])
        print(f"{red}{text}{RESET}", file=stderr, end="")

    @classmethod
    def print_step(cls, step_nb: int, moves: list[Move]) -> None:
        print(f"{BOLD}------- Turn nb: {step_nb} -------{RESET}")
        for move in moves:
            print(cls.format_move(move), end=" ")
        print("\n")

    @classmethod
    def build_ansi(cls, rgb: RGB) -> str:
        r, g, b = rgb
        return f"\033[38;2;{r};{g};{b}m"

    @classmethod
    def _paint_zone(cls, name: str, color: Optional[str]) -> str:
        if color == "rainbow":
            rainbow: list[str] = []
            for i, c in enumerate(name):
                rgb = RAINBOW[i % len(RAINBOW)]
                rainbow.append(cls.build_ansi(rgb) + c)
            return "".join(rainbow) + RESET
        elif color is None or color not in COLORS:
            return cls.build_ansi(DEFAULT_ZONE_COLOR) + name + RESET
        else:
            return cls.build_ansi(COLORS[color]) + name + RESET

    @classmethod
    def format_move(cls, move: Move) -> Optional[str]:
        id = ("D" + str(move.id)) + "-"
        zone = ""

        if move.zone is not None:
            zone = cls._paint_zone(move.zone.name, move.zone.color)
            return id + zone
        elif move.connection is not None:
            return id + BOLD +"conn(" + move.connection + ")" + RESET
