#!/usr/bin/env python3

class PlantError(Exception):
    def __init__(self, msg: str = "Unknow plant error") -> None:
        super().__init__(msg)


def water_plant(plant_name: str) -> None:
    if not plant_name == plant_name.capitalize():
        raise PlantError(f"Invalid plant name to water: '{plant_name}'")
    print(f"Watering {plant_name}: [OK]")


def test_watering_system(lst: list) -> None:
    print("Opening watering systems")
    try:
        for x in lst:
            water_plant(x)
    except Exception as error:
        print(f"Caught {error.__class__.__name__}: {error}")
        print(".. ending tests and returning to main")
    finally:
        print("Closing watering system")


if __name__ == "__main__":
    print("=== Garden Watering System ===", end="\n\n")

    print("Testing valid plants...")
    test_watering_system(["Tomato", "Lettuce", "Carrots"])

    print("\nTesting invalid plants...")
    test_watering_system(["Tomato", "lettuce", "Carrots"])

    print("\nCleanup always happends, even with errors")
