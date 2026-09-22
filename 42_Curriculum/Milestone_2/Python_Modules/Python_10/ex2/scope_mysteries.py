#!/usr/bin/env python3


from typing import Callable, Any


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


def mage_counter() -> Callable[[], int]:
    count = 0

    def counter() -> int:
        nonlocal count
        count += 1
        return count
    return counter


def spell_accumulator(initial_power: int) -> Callable[[int], int]:
    acumulated_power = initial_power

    def acumulate(power: int) -> int:
        nonlocal acumulated_power
        acumulated_power += power
        return acumulated_power
    return acumulate


def enchantment_factory(enchantment_type: str) -> Callable[[str], str]:
    return lambda item: enchantment_type + " " + item


def memory_vault() -> dict[str, Callable[..., Any]]:
    vault = {}

    def store(key: str, value: int) -> None:
        vault.update({key: value})

    def recall(key: str) -> int | str:
        return vault[key] if key in vault else "Memory not found"

    return {"store": store, "recall": recall}


if __name__ == "__main__":
    print(C.U, end="")
    C().msg("Bo", "Testing mage counter:")
    counter_a = mage_counter()
    counter_b = mage_counter()

    print(f"counter_a call 1: {counter_a()}")
    print(f"counter_b call 1: {counter_b()}")
    print(f"counter_a call 2: {counter_a()}")

    print(C.U, end="")
    C().msg("Bo", "\nTesting spell acumulator:")
    power = spell_accumulator(100)

    print(f"Base 100, add 20: {power(20)}")
    print(f"Base 100, add 30: {power(30)}")
    print(f"Base 100, add 50: {power(50)}")

    print(C.U, end="")
    C().msg("Bo", "\nTesting enchanting items:")
    flame_enchant = enchantment_factory("Flaming")
    water_enchant = enchantment_factory("Respiration")
    wind_enchant = enchantment_factory("Speed")

    print("Enchanting sword with flame enchant:",
          f"{flame_enchant("sword")}")
    print("Enchanting helmet with water enchant:",
          f"{water_enchant("helmet")}")
    print("Enchanting boots with wind enchant:",
          f"{wind_enchant("boots")}")

    print(C.U, end="")
    C().msg("Bo", "\nTesting memory vault:")
    vault = memory_vault()

    print("Store 'secret' = 42")
    vault["store"]("secret", 42)
    print(f"Recall 'secret' : {vault["recall"]("secret")}")
    print(f"Recall 'unknown' : {vault["recall"]("unknown")}")
