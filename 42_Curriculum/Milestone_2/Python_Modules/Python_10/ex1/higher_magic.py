#!/usr/bin/env python3


from typing import Callable


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


class Grimoire:
    @staticmethod
    def heal(t: str, p: int) -> str:
        return f"Heal restores {t} for {p} HP"

    @staticmethod
    def fireball(t: str, p: int) -> str:
        return f"Fireball deals {p} fire damage to {t}"

    @staticmethod
    def freeze(t: str, p: int) -> str:
        return f"Freeze immobilizes {t} with {p} ice p"

    @staticmethod
    def lightning(t: str, p: int) -> str:
        return f"Lightning strikes {t} for {p} damage"

    @staticmethod
    def shield(t: str, p: int) -> str:
        return f"Shield grants {t} {p} defense"

    @staticmethod
    def dark_curse(t: str, p: int) -> str:
        return f"Dark Curse weakens {t} by {p} points"

    @staticmethod
    def condition(num1: int, num2: int) -> bool:
        return True if num1 == num2 else False


def spell_combiner(spell1: Callable[[str, int], str],
                   spell2: Callable[[str, int], str]) -> \
        Callable[[str, int], tuple[str, str]]:
    return lambda t, p: (spell1(t, p), spell2(t, p))


def p_amplifier(base_spell: Callable[[str, int], str],
                multiplier: int) -> Callable[[str, int], str]:
    return lambda t, p: base_spell(t, p * multiplier)


def conditional_caster(condition: Callable[[int, int], bool],
                       spell: Callable[[str, int], str]) -> \
        Callable[[int, int, str, int], str]:
    return lambda n1, n2, t, p: spell(t, p) if condition(n1, n2) else \
                        "Spell fizzled"


def spell_sequence(spells: list[Callable[[str, int], str]]) -> \
        Callable[[str, int], list[str]]:
    return lambda t, p: [spell(t, p) for spell in spells]


if __name__ == "__main__":
    test_target = ['Dragon', 'Goblin', 'Wizard', 'Knight']

    print(C.U, end="")
    C().msg("Bo", "Testing spell combiner:")
    combined = spell_combiner(Grimoire.dark_curse,
                              Grimoire.heal)
    print(combined(test_target[2], 5))

    print(C.U, end="")
    C().msg("Bo", "\nTesting spell amplifier:")
    amplifier = p_amplifier(Grimoire.fireball, 5)
    print(amplifier(test_target[0], 20))

    print(C.U, end="")
    C().msg("Bo", "\nTesting conditional caster:")
    cast = conditional_caster(Grimoire.condition, Grimoire.shield)
    print(cast(2, 2, test_target[1], 7))

    print(C.U, end="")
    C().msg("Bo", "\nTesting spell list:")
    spell_list: list[Callable[[str, int], str]] = [
        Grimoire.dark_curse,
        Grimoire.fireball,
        Grimoire.freeze,
        Grimoire.heal,
        Grimoire.lightning,
        Grimoire.shield
    ]
    sequence = spell_sequence(spell_list)
    print(sequence(test_target[3], 5))
