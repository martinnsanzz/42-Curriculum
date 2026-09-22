#!/usr/bin/env python3


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
    C().msg("H", "=== Kaboon 1===")
    print("Access to alchemy/grimoire/dark_spellbook.py directly")
    print("Testing import now: ", end="")
    C().msg("F", "THIS WILL RAISE AN UNCAUGHT EXCEPTION")

    try:
        import alchemy.grimoire.dark_spellbook as dark
        is_valid = dark.dark_spell_record("Fantasy", "frogs, wind and fire")
    except Exception as error:
        C().msg("F", str(error))
