#!/usr/bin/env python3

class Plant:
    def __init__(self, name: str, height: float, plant_age: int,
                 growth_rate: float) -> None:
        self.name = name
        self.height = height
        self.plant_age = plant_age
        self.growth_rate = growth_rate
        self.week_growth = 0.0

    def show(self) -> None:
        print(f"{self.name}: {round(self.height, 1)}cm, ", end="")
        print(f"{self.plant_age} days old")

    def age(self) -> None:
        self.plant_age += 1
        self.height += self.growth_rate
        self.week_growth += self.growth_rate

    def grow(self, days: int) -> None:
        self.show()
        for x in range(1, days + 1):
            self.age()
            print(f"=== Day {x} ===")
            self.show()
            if (x % 7 == 0):
                print(f"Growth this week: {round(self.week_growth, 1)}cm")
                self.week_growth = 0


if __name__ == "__main__":
    plant_1: Plant = Plant("Rose", 25.0, 30, 0.0)
    plant_2: Plant = Plant("Oak", 200.0, 365, 0.0)
    plant_3: Plant = Plant("Cactus", 5.0, 90, 0.0)
    plant_4: Plant = Plant("Sunflower", 80.0, 45, 0.0)
    plant_5: Plant = Plant("Fern", 15.0, 120, 0.0)
    plants_lst = [plant_1, plant_2, plant_3, plant_4, plant_5]

    print("=== Plant Factory Output ===")
    for x in plants_lst:
        print("Created: ", end="")
        x.show()
