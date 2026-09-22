#!/usr/bin/env python3


from typing import cast, Protocol
import ex1


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


class HealableCreature(Protocol):
    def describe(self) -> str: ...
    def attack(self) -> str: ...
    def heal(self) -> str: ...


class TransformCreature(Protocol):
    def describe(self) -> str: ...
    def attack(self) -> str: ...
    def transform(self) -> str: ...
    def revert(self) -> str: ...


def heal_factory_test(factory: ex1.HealingCreatureFactory) -> None:
    base_creature = cast(HealableCreature, factory.create_base())
    evolved_creature = cast(HealableCreature, factory.create_evolved())

    C().msg("Bo", " base: ")
    print(base_creature.describe())
    print(base_creature.attack())
    print(base_creature.heal())
    C().msg("Bo", " evolved: ")
    print(evolved_creature.describe())
    print(evolved_creature.attack())
    print(evolved_creature.heal())


def transform_factory_test(factory: ex1.TransformCreatureFactory) -> None:
    base_creature = cast(TransformCreature, factory.create_base())
    evolved_creature = cast(TransformCreature, factory.create_evolved())

    C().msg("Bo", " base: ")
    print(base_creature.describe())
    print(base_creature.attack())
    print(base_creature.transform())
    print(base_creature.attack())
    print(base_creature.revert())
    C().msg("Bo", " evolved: ")
    print(evolved_creature.describe())
    print(evolved_creature.attack())
    print(evolved_creature.transform())
    print(evolved_creature.attack())
    print(evolved_creature.revert())


if __name__ == "__main__":
    heal_factory = ex1.HealingCreatureFactory()
    transform_factory = ex1.TransformCreatureFactory()

    C().msg("H", "=== Testing Creature with healing \
capability ===")
    heal_factory_test(heal_factory)

    C().msg("H", "=== Testing Creature with Transform \
capability ===")
    transform_factory_test(transform_factory)
