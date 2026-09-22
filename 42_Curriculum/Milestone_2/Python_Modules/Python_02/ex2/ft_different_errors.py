#!/usr/bin/env python3


def garden_operations(operation_number: int) -> str:
    print(f"Testing operation {operation_number}...")

    if operation_number == 0:
        int("abc")
    elif operation_number == 1:
        12 / 0
    elif operation_number == 2:
        open('/non/existent/file')
    elif operation_number == 3:
        "Hello" + 42
    return "Operation completed successfully"


def test_error_types() -> None:
    for x in [0, 1, 2, 3, 4]:
        try:
            print(garden_operations(x))
        except (ValueError, ZeroDivisionError,
                FileNotFoundError, TypeError) as error_msg:
            print(f"Caught {error_msg.__class__.__name__}: ", end="")
            print(error_msg)


if __name__ == "__main__":
    print("=== Garden Error Types Demo ===")
    test_error_types()
    print("\nAll error types tested successfully")
