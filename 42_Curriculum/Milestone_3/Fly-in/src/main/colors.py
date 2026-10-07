# Built-in modules
from enum import StrEnum
from typing import Any
import sys


class Color(StrEnum):
    RED = "\033[31m"
    GREEN = "\033[32m"
    YELLOW = "\033[33m"
    BLUE = "\033[34m"
    MAGENTA = "\033[35m"
    CYAN = "\033[36m"
    BOLD = "\033[1m"
    RESET = "\033[0m"


def paint(text: str, color: Color, stream: Any =sys.stdout) -> str:
    if not stream.isatty():
        return text
    return f"{color}{text}{Color.RESET}"