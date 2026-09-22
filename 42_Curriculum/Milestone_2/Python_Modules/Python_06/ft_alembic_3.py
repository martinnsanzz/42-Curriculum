#!/usr/bin/env python3


from alchemy.elements import create_air


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

    def msg(self, color: str, msg: str):
        print(getattr(self, color) + msg + C.E)


if __name__ == "__main__":
    C().msg("H", "=== Alembic 3 ===")
    print("Accessing alchemy/elements.py using 'from ... \
import ...' structure")
    print("Testing create_air: ", end="")
    C().msg("C", create_air())
