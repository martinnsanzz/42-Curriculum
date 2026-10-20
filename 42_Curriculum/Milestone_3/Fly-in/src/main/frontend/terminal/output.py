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

ERROR = "------ERROR FOUND------"

class TerminalPrinter:
    """Print simulation output and errors to the terminal using ANSI colors.

    All methods are class methods and no instance state is used, so the
    class is used as a namespace and is not meant to be instantiated.
    """
    @classmethod
    def print_error(cls, error: str) -> None:
        """Print an error message to stderr, in red."""
        if not stderr.isatty():
            print(ERROR, file=stderr)
        red = cls.build_ansi(COLORS["red"])
        print(f"{red}{ERROR}\n{error}{RESET}", file=stderr, end="")

    @classmethod
    def output_simulation(cls, steps: list[list[Move]]) -> None:
        """Print the simulation turn by turn to stdout.

        For each turn, print a bold `Turn nb: <n>` header (1-based) and then
        all moves of that turn on one line, separated by spaces, followed
        by a blank line.
        """
        for i, steps in enumerate(steps):
            print(f"{BOLD}------- Turn nb: {i + 1} -------{RESET}")
            for move in steps:
                print(cls.format_move(move), end=" ")
            print("\n")

    @classmethod
    def build_ansi(cls, rgb: RGB) -> str:
        """Build the ANSI escape sequence for a 24-bit foreground color."""
        r, g, b = rgb
        return f"\033[38;2;{r};{g};{b}m"

    @classmethod
    def _paint_zone(cls, name: str, color: Optional[str]) -> str:
        """Return a zone name wrapped in the ANSI color for `color`.
        
        Use one color per character for `"rainbow"`, cycling through
        `RAINBOW`. Use `DEFAULT_ZONE_COLOR` when `color` is `None` or not a
        key of `COLORS`. Otherwise use `COLORS[color]`.

        Returns:
            str: The colored text, always ending with `RESET`.
        """
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
    def format_move(cls, move: Move) -> str:
        """Format one drone move as a printable string.

        The text starts with `D<id>-`. It is followed by the colored zone
        name if the move has a zone, or by the connection name in bold
        (`conn(<connection>)`) if it has a connection. The zone takes
        precedence if both are set.

        Returns:
            str: The formatted move
        """
        id = ("D" + str(move.id)) + "-"
        zone = ""

        if move.zone is not None:
            zone = cls._paint_zone(move.zone.name, move.zone.color)
            return id + zone
        return id + BOLD +"conn(" + move.connection + ")" + RESET
