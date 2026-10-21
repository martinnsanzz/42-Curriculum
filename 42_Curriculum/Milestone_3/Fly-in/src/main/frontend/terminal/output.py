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

DEFAULT_HUB_COLOR = (128, 128, 128)

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
    def output_simulation(cls, total_steps: list[list[Move]]) -> None:
        """Print the simulation turn by turn to stdout.

        For each turn, print a bold `Turn nb: <n>` header (1-based) and then
        all moves of that turn on one line, separated by spaces, followed
        by a blank line.
        """
        for i, t_steps in enumerate(total_steps):
            print(f"{BOLD}------- Turn nb: {i + 1} -------{RESET}")
            for move in t_steps:
                print(cls.format_move(move), end=" ")
            print("\n")

    @classmethod
    def build_ansi(cls, rgb: RGB) -> str:
        """Build the ANSI escape sequence for a 24-bit foreground color."""
        r, g, b = rgb
        return f"\033[38;2;{r};{g};{b}m"

    @classmethod
    def _paint_hub(cls, name: str, color: Optional[str]) -> str:
        """Return a hub name wrapped in the ANSI color for `color`.

        Use one color per character for `"rainbow"`, cycling through
        `RAINBOW`. Use `DEFAULT_hub_COLOR` when `color` is `None` or not a
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
            return cls.build_ansi(DEFAULT_HUB_COLOR) + name + RESET
        else:
            return cls.build_ansi(COLORS[color]) + name + RESET

    @classmethod
    def format_move(cls, move: Move) -> str:
        """Format one drone move as a printable string.

        The text starts with `D<id>-`. It is followed by the colored hub
        name if the move has a hub, or by the connection name in bold
        (`conn(<connection>)`) if it has a connection. The hub takes
        precedence if both are set.

        Returns:
            str: The formatted move
        """
        id = ("D" + str(move.id)) + "-"
        hub = ""

        if move.hub is not None:
            hub = cls._paint_hub(move.hub.name, move.hub.metadata.color)
            return id + hub
        elif move.connection is not None:
            return id + BOLD + "conn(" + move.connection + ")" + RESET
        return ""
