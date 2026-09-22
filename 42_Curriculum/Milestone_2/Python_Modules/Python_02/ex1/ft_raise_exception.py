#!/usr/bin/env python3


def input_temperature(temp_str: str) -> int:
    temp_int = int(temp_str)

    if temp_int < 0:
        raise ValueError(f"{temp_int}°C is too cold for plants (min 0°C)")
    elif temp_int > 40:
        raise ValueError(f"{temp_int}°C is too hot for plants (max 40°C)")
    elif temp_str == "-0":
        raise ValueError("-0 is not a number")
    return temp_int


def test_temperature(temp_str: str) -> None:
    print(f"\nInput data is '{temp_str}'")

    try:
        print(f"Temperature is now {input_temperature(temp_str)}°C")
    except ValueError as error_msg:
        print("Caught input_temperature error: ", end="")
        print(error_msg)


if __name__ == "__main__":
    print("=== Garden Temperature ===")
    test_temperature("25")
    test_temperature("abc")
    test_temperature("100")
    test_temperature("-50")
    print("\nAll tests completed - program didn't crash!")
