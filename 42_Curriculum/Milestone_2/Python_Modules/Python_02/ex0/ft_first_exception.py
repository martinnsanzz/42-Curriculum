#!/usr/bin/env python3


def input_temperature(temp_str: str) -> int:
    return (int(temp_str))


def test_temperature(temp_str: str) -> None:
    print(f"\nInput data is '{temp_str}'")

    try:
        print(f"Temperature is now {input_temperature(temp_str)}°C")
    except ValueError:
        print("Caught input_temperature error: invalid ", end="")
        print(f"literal for int() with base 10: {temp_str}")


if __name__ == "__main__":
    print("=== Garden Temperature ===")
    test_temperature("25")
    test_temperature("abc")
    print("\nAll tests completed - program didn't crash!")
