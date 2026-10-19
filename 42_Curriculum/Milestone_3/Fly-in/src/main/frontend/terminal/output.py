# Built-in modules
from typing import Optional
from sys import stderr

# Local Modules
from ...backend import Move

RGB = tuple[int, int, int]

COLORS: dict[str, RGB] = {
        "black": (40, 36, 1), "blue": (76, 118, 128), "brown": (104, 63, 46),
        "crimson": (153, 0, 0), "cyan": (0, 255, 255),
        "darkred": (162, 85, 74), "gold": (255, 200, 0),
        "lime": (194, 209, 27), "magenta": (237, 106, 90),
        "maroon": (132, 73, 56), "orange": (246, 153, 45),
        "purple": (150, 0, 205), "red": (255, 0, 0),
        "violet": (84, 22, 180), "yellow": (255, 231, 0),
        "green": (124, 252, 0), "gray": (127, 127, 127),
        }

DEFAULT_ZONE_COLOR = (128, 128, 128)

RAINBOW: list[RGB] = [
        COLORS["red"], COLORS["orange"], COLORS["yellow"],
        COLORS["green"], COLORS["blue"], COLORS["violet"]
        ]

RESET = "\033[0m"
BOLD = "\033[1m"

class TerminalPrinter:
    @classmethod
    def print_error(cls, text: str) -> None:
        if not stderr.isatty():
            print(text, file=stderr)
        red = cls.build_ansi(COLORS["black"])
        print(f"{red}{text}{RESET}", file=stderr, end="")

    @classmethod
    def print_step(cls, step_nb: int, moves: list[Move]) -> None:
        print(f"{BOLD}------- Step nb: {step_nb} -------{RESET}")
        for move in moves:
            print(cls.format_move(move), end=" ")
        print()

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
            return id + move.connection
