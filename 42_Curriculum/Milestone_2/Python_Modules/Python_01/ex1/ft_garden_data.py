#!/usr/bin/env python3

class Plant:
    def __init__(self, name: str, height: int, age: int) -> None:
        self.name = name
        self.height = height
        self.age = age

    def show(self) -> str:
        return f"{self.name}: {self.height}cm, {self.age} days old"


if __name__ == '__main__':
    plant_1: Plant = Plant("Rose", 25, 30)
    plant_2: Plant = Plant("Sunflower", 80, 45)
    plant_3: Plant = Plant("Cactus", 15, 120)

    print("=== Garden Plant Registry ===")
    print(plant_1.show())
    print(plant_2.show())
    print(plant_3.show())
