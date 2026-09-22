#!/usr/bin/env python3


from typing import Callable, Any
from functools import reduce, partial, lru_cache, singledispatch
import operator as op


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


class CustomError(Exception):
    def __init__(self, msg: str) -> None:
        super().__init__(msg)


def spell_reducer(spells: list[int], operation: str) -> int:
    op_dict = {"add": op.add, "multiply": op.mul,
               "max": lambda a, b: a if op.gt(a, b) else b,
               "min": lambda a, b: a if op.lt(a, b) else b}
    if not spells:
        return 0
    elif operation in op_dict:
        return reduce(op_dict[operation], spells)
    else:
        raise CustomError("Unknown operation type. Support operation \
'add', 'multiply', 'max', 'min'")


def partial_enchanter(base_enchantment: Callable[[int, str, str], str]) -> \
        dict[str, Callable[[str], str]]:
    fire = partial(base_enchantment, 50, "fire")
    ice = partial(base_enchantment, 50, "ice")
    lighting = partial(base_enchantment, 50, "lighting")

    return {"fire": fire, "ice": ice, "lighting": lighting}


def base_enchantment(power: int, element: str, target: str) -> str:
    return f"{target} was dealt {power} damage with {element}"


@lru_cache(maxsize=None)
def memoized_fibonacci(n: int) -> int:
    if n <= 1:
        return n
    return memoized_fibonacci(n - 1) + memoized_fibonacci(n - 2)


def spell_dispatcher() -> Callable[[Any], str]:
    @singledispatch
    def cast(spell: Any) -> str:
        return "Unknown spell type"

    @cast.register(int)
    def _(spell: int) -> str:
        return f"{spell} damage"

    @cast.register(str)
    def _(spell: str) -> str:
        return f"{spell} spell"

    @cast.register(list)
    def _(spell: list[Any]) -> str:
        return f"{len(spell)} " + ("spell" if len(spell) <= 1 else "spells")

    return cast


if __name__ == "__main__":
    spells: list[int] = [1, 101, 3, 100, -1, 6, 7, 8, 1023]

    print(C.U, end="")
    C().msg("Bo", "\nTesting spell reducer:")
    print(f"Sum: {spell_reducer(spells, "add")}")
    print(f"Multiply: {spell_reducer(spells, "multiply")}")
    print(f"Max: {spell_reducer(spells, "max")}")
    print(f"Min: {spell_reducer(spells, "min")}")
    print("Divide: ", end="")
    try:
        print(spell_reducer(spells, "unknown"))
    except CustomError as error:
        C().msg("F", str(error))

    print(C.U, end="")
    C().msg("Bo", "\nTesting partial enchanter:")
    enchantments_dict = partial_enchanter(base_enchantment)
    print(enchantments_dict["fire"]('Goblin'))
    print(enchantments_dict["ice"]('Dragon'))
    print(enchantments_dict["lighting"]('Witch'))

    print(C.U, end="")
    C().msg("Bo", "\nTesting memoized fibonacci:")
    print(f"Fib(0): {memoized_fibonacci(0)}")
    print(f"Fib(1): {memoized_fibonacci(1)}")
    print(f"Fib(10): {memoized_fibonacci(10)}")
    print(f"Fib(15): {memoized_fibonacci(15)}")
    print(memoized_fibonacci.cache_info())

    print(C.U, end="")
    C().msg("Bo", "\nTesting spell dispatcher:")
    spell = spell_dispatcher()
    print(f"Damage spell: {spell(42)}")
    print(f"Enchantment: {spell("Fireball")}")
    print(f"Multi-cast: {spell(["Fireball", "Freeze", "Tornado"])}")
    print(spell(4.2))
