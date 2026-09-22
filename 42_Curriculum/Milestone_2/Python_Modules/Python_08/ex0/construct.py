#!/usr/bin/env python3


import sys
import os


class C:
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


def outside_venv() -> None:
    print("\nMATRIX STATUS: ", end="")
    C().msg("F", "You're still plugged in...")

    print("\nCurrent Python: ", end="")
    C().msg("C", sys.executable)
    print("Virtual Environment: ", end="")
    C().msg("F", "None detected")

    print("\nTo enter the construct, run:"
          "\n```bash"
          "\npython -m venv matrix_env"
          "\nsource matrix_env/bin/activate # On Unix"
          "\nmatrix_env/Scripts/activate # On Windows"
          "\n```"
          "\nThen run this program again.")


def inside_venv() -> None:
    print("\nMATRIX STATUS: ", end="")
    C().msg("G", "Welcome to the construct !!")

    print("\nCurrent Python: ", end="")
    C().msg("C", sys.executable)
    print("Virtual Environment: ", end="")
    C().msg("C", os.path.basename(os.environ['VIRTUAL_ENV']))
    print("Environment Path: ", end="")
    C().msg("C", os.environ['VIRTUAL_ENV'])

    print("\nSUCCESS: You're in an isolated environment!"
          "Safe to install packages without affecting"
          "the global system.")

    print("\nPackage installation path: ")
    C().msg("C", sys.path[4])


if __name__ == "__main__":
    if sys.prefix != sys.base_prefix:
        inside_venv()
    else:
        outside_venv()
