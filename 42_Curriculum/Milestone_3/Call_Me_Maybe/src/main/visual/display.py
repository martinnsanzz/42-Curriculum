# Built-in modules
from subprocess import call
import os
from typing import TYPE_CHECKING
from json import dumps

# Local modules
from .ascii_art import CALL_ME_MAYBE_ASCII

if TYPE_CHECKING:
    from ..classes import Prompt


class C:
    """Format regular strings based on color input"""
    H = '\033[95m'  # Header
    B = '\033[94m'  # Blue
    C = '\033[96m'  # Cyan
    G = '\033[92m'  # Green
    W = '\033[93m'  # Warning
    F = '\033[91m'  # Fail
    E = '\033[0m'  # End
    Bo = '\033[1m'  # Bold
    U = '\033[4m'

    def msg(self, color: str, msg: str) -> None:
        print(getattr(self, color) + msg + C.E)


def display_title() -> None:
    """Clears terminal screen and displays title"""
    call('clear' if os.name == 'posix' else 'cls')
    print()
    C().msg("H", str(CALL_ME_MAYBE_ASCII))


def display_prompt_info(prompt_obj: 'Prompt', index: int,
                        error_msg: str, prompt_time: float) -> None:
    """Display prompt information with color when called"""
    C().msg("Bo", f"\n================ Prompt {index} ========="
            "===========\n\n")

    print(f"\033[93mPrompt: \033[0m'{prompt_obj.prompt}'")

    if not error_msg:
        print(f"\033[93mFunction name: \033[0m'{prompt_obj.name}'")
        print(f"\033[93mParameters: \033[0m'{dumps(prompt_obj.parameters)}",
              end="\n\n")
        print(f"\033[93mPrompt time: \033[92m'{round(prompt_time, 2)}'\033[0m")
    else:
        print()
        C().msg("F", str(error_msg))
    C().msg("Bo", "\n==============================================")


def display_total_info(error_log: int, total_prompt: int,
                       total_time: float) -> None:
    """Display total information with color when called"""
    C().msg("Bo", "\n================ Stats ========================\n\n")
    print(f"\033[93mTotal time: \033[92m'{round(total_time, 2)}'\033[0m")

    if error_log:
        print("\033[93mNumber of errors: "
              f"\033[91m'{error_log}/{total_prompt}'\033[0m")
    else:
        print(f"\033[93mNumber of errors: \033[92m'0/{total_prompt}'\033[0m")
    C().msg("Bo", "\n==============================================\n\n")
