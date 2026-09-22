#!/usr/bin/env python3


from functools import wraps
from typing import Callable, Any
from time import perf_counter, sleep
from random import randint


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


def spell_timer(func: Callable[..., Any]) -> Callable[..., Any]:
    @wraps(func)
    def wrapper(*args: Any, **kwargs: Any) -> Any:
        print(f"Casting {func.__name__}")
        elapsed_time = perf_counter()
        result = func(*args, **kwargs)
        elapsed_time = perf_counter() - elapsed_time
        print(f"Spell completed in {round(elapsed_time, 3)}")
        return result
    return wrapper


def power_validator(min_power: int) -> Callable[..., Any]:
    def validate(func: Callable[..., Any]) -> str | Callable[..., Any]:
        @wraps(func)
        def wrapper(*args: Any, **kwargs: Any) -> str | Any:
            if func.__code__.co_varnames[0] != "power":
                raise CustomError("First arg of function should be 'power'")
            elif args[0] >= min_power:
                return func(*args, **kwargs)
            else:
                return "Insufficient power for this spell"
        return wrapper
    return validate


def retry_spell(max_attempts: int) -> Callable[..., Any]:
    def validate(func: Callable[..., Any]) -> str | Callable[..., Any]:
        @wraps(func)
        def wrapper(*args: Any, **kwargs: Any) -> str | Any:
            i = 1
            while (i <= max_attempts):
                try:
                    return func(*args, **kwargs)
                except CustomError:
                    print(f"Spell failed, retrying... \
(attempt {i}/{max_attempts})")
                    sleep(1)
                i += 1
            return f"Spell casting failed after {max_attempts} attempts"
        return wrapper
    return validate


class MageGuild:
    @staticmethod
    def validate_mage_name(name: str) -> bool:
        is_valid = all(char.isalpha() or char.isspace() for char in name)
        if len(name) >= 3 and is_valid:
            return True
        return False

    def cast_spell(self, spell_name: str, power: int) -> Any:
        @power_validator(10)
        def cast(power: int = power) -> str:
            return f"Successfully cast {spell_name} with {power} power"
        return cast(power)


@spell_timer
def slow_spell(n: int) -> int:
    total = 0
    for i in range(n):
        total += i
    return total


@power_validator(50)
def power(power: int) -> str:
    return f"Power is {power}"


@retry_spell(3)
def spell_cast(spell: str, power: int) -> str:
    power_required = randint(0, 100)
    if power_required <= power:
        return f"Dealt {power} damage using {spell}"
    else:
        raise CustomError("Not enough power")


if __name__ == "__main__":
    print(C.U, end="")
    C().msg("Bo", "Testing spell timer...")
    slow_spell(50000000)

    print(C.U, end="")
    C().msg("Bo", "\nTesting power validator...")
    print(power(50))
    print(power(49))

    print(C.U, end="")
    C().msg("Bo", "\nTesting retry spell...")
    print(spell_cast("Fireball", 33))

    print(C.U, end="")
    C().msg("Bo", "\nTesting name validation...")
    print(MageGuild().validate_mage_name("Martin the mage"))
    print(MageGuild().validate_mage_name("Marton the ****"))

    print(C.U, end="")
    C().msg("Bo", "\nTesting casting spell...")
    print(MageGuild().cast_spell("Fireball", 11))
    print(MageGuild().cast_spell("Fireball", 9))
