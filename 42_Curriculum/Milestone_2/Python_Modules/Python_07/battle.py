#!/usr/bin/env python3


import ex0


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


def factory_test(factory: ex0.CreatureFactory) -> None:
    base_creature = factory.create_base()
    evolved_creature = factory.create_evolved()

    print(base_creature.describe())
    print(base_creature.attack())
    print(evolved_creature.describe())
    print(evolved_creature.attack())


def test_battle(flame_factory: ex0.FlameFactory,
                aqua_factory: ex0.AquaFactory) -> None:
    fire_base = flame_factory.create_base()
    aqua_base = aqua_factory.create_base()

    print(fire_base.describe())
    C().msg("W", "VS")
    print(aqua_base.describe())
    C().msg("Bo", fire_base.attack())
    C().msg("Bo", aqua_base.attack())


if __name__ == "__main__":
    flame_factory = ex0.FlameFactory()
    aqua_factory = ex0.AquaFactory()

    C().msg("H", "=== Testing Fire Factory ===")
    factory_test(flame_factory)

    C().msg("H", "\n=== Testing Water Factory ===")
    factory_test(aqua_factory)

    C().msg("H", "\n=== Testing Battle ===")
    test_battle(flame_factory, aqua_factory)
