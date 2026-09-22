#!/usr/bin/env python3


import alchemy


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
    C().msg("H", "=== Alembic 4 ===")
    print("Accessing the alchemy module using 'import alchemy'")
    print("Testing create_air: ", end="")
    C().msg("C", alchemy.create_air())

    print("\nNow show that not all functions can be reached")
    C().msg("W", "This will raise an exception!")

    # Catching the error
    try:
        print("Testing the hidden create_earth: ", end="")
        print(alchemy.create_earth())
    except Exception as error:
        C().msg("F", str(error).capitalize())

    # Without catching the error
    # print("\n\nTesting the hidden create_earth: ", end="")
    # print(alchemy.create_earth())
