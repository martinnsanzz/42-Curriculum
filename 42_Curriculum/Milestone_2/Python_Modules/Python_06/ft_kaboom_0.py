#!/usr/bin/env python3


import alchemy.grimoire


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
    C().msg("H", "=== Kaboon 0 ===")
    print("Using grimoire module directly")
    print("Testing record light spell: ", end="")
    is_valid = alchemy.grimoire.light_spell_record("Fantasy", "Earth, \
wind and fire")

    if "INVALID" in is_valid:
        C().msg("F", is_valid)
    else:
        C().msg("G", is_valid)
