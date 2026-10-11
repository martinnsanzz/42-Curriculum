# Built-in modules
from enum import StrEnum
from sys import stdout, stderr


class Color(StrEnum):
    RED = "\033[31m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    BLUE = "\033[34m"
    MAGENTA = "\033[35m"
    CYAN = "\033[36m"
    BOLD = "\033[1m"
    RESET = "\033[0m"


def paint(text: str, color: Color) -> str:
    if not stdout.isatty():
        return text
    return f"{color}{text}{Color.RESET}"

def print_error(text: str) -> None:
    if not stderr.isatty():
        print(text, file=stderr)
    print(f"{Color.RED}{text}{Color.RESET}", file=stderr, end="")