# Built-in modules
from typing import Optional
from sys import stderr, stdout

# Local Modules
from ...backend import Move

RGB = tuple[int, int, int]

COLORS: dict[str, RGB] = {
        "black": (0, 0, 0), "blue": (0, 0, 255), "brown": (153, 51, 0),
        "crimson": (204, 0, 51), "cyan": (0, 255, 255),
        "darkred": (102, 0, 0), "gold": (255, 204, 0),
        "lime": (0, 255, 0), "magenta": (255, 0, 255),
        "maroon": (51, 0, 0), "orange": (255, 153, 0),
        "purple": (102, 0, 102), "red": (255, 0, 0),
        "violet": (204, 102, 204), "yellow": (255, 255, 0),
        "green": (153, 204, 102), "gray": (102, 102, 102),
        }

DEFAULT_HUB_COLOR = (102, 102, 102)

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
    def print_msg(cls, msg: str, color: str = "None") -> None:
        """Print a message to stdout in the specified color.
        
        If color is not present in `COLORS` it will provide a default
        hub color `grey`.
        """
        if not stdout.isatty():
            print(msg)
        if color in COLORS:
            ansi = cls.build_ansi(COLORS[color])
            print(f"{ansi}{msg}{RESET}")
        else:
            ansi = cls.build_ansi(DEFAULT_HUB_COLOR)
            print(f"{ansi}{msg}{RESET}")

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

    @staticmethod
    def _to_cube(channel: int) -> int:
        """Scale a 0-255 channel to the 0-5 range of the 256-color cube."""
        return (channel * 5 + 127) // 255

    @classmethod
    def build_ansi(cls, rgb: RGB) -> str:
        """Build the ANSI escape sequence for a 256-color foreground."""
        r, g, b = (cls._to_cube(c) for c in rgb)
        index = 16 + 36 * r + 6 * g + b
        return f"\033[38;5;{index}m"

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
