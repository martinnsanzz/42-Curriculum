#!/usr/bin/env python3


class GardenError(Exception):
    def __init__(self, msg: str = "Unknown garden error") -> None:
        super().__init__(msg)


class PlantError(GardenError):
    def __init__(self, msg: str = "Unknow plant error") -> None:
        super().__init__(msg)


class WaterError(GardenError):
    def __init__(self, msg: str = "Unknown water error") -> None:
        super().__init__(msg)


def catch_errors(cls: type) -> None:
    try:
        raise (cls)
    except Exception as error:
        print(f"Caught {error.__class__.__name__}: {error}")


def garden_errors(cls: type) -> None:
    try:
        raise (cls)
    except GardenError as error:
        print(f"Caught GardenError: {error}")
        # print(f"Caught {error.__class__.__bases__[0].__name__}: {error}")


def print_errors():
    print("\nTesting PlantError...")
    catch_errors(PlantError("The tomato plant is wilting!"))
    print("\nTesting WaterError...")
    catch_errors(WaterError("Not enough water in the tank!"))
    print("\nTesting catching all garden errors...")
    garden_errors(PlantError("The tomato plant is wilting!"))
    garden_errors(WaterError("Not enough water in the tank!"))


if __name__ == "__main__":
    print("=== Custom Garden Errors Demo ===")
    print_errors()
    print("\nAll custom errors types work correctly")
