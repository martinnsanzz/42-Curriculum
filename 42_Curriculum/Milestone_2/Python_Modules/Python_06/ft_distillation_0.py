#!/usr/bin/env python3


from alchemy.potions import healing_potion, strength_potion


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
    C().msg("H", "=== Distillation 0 ===")
    C().msg("Bo", "Direct access to alchemy/potions.py")
    print("Testing strength_potion: ", end="")
    C().msg("F", strength_potion())

    print("\nTesting healing_potion: ", end="")
    C().msg("G", healing_potion())
