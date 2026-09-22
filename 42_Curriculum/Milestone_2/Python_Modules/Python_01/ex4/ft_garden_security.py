#!/usr/bin/env python3

class Plant:
    def __init__(self, name: str, height: float, plant_age: int,
                 growth_rate: float) -> None:
        self.name = name
        self._height = (height, 15.0)[height < 0]
        self._plant_age = (plant_age, 10)[plant_age < 0]
        self.growth_rate = growth_rate
        self.week_growth = 0.0
        if (height < 0):
            print("Height can't be negative")
        if (plant_age < 0):
            print("Age can't be negative")

    def show(self) -> None:
        print(f"{self.name}: {round(self._height, 1)}cm, ", end="")
        print(f"{self._plant_age} days old")

    def age(self) -> None:
        self._plant_age += 1
        self._height += self.growth_rate
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

    def set_height(self, amount: float):
        if amount > 0:
            self._height = amount
        else:
            print(f"{self.name}: Error, height can't be negative")
            print("Height update rejected")

    def set_age(self, amount: int):
        if amount > 0:
            self._plant_age = amount
        else:
            print(f"{self.name}: Error, age can't be negative")
            print("Age update rejected")

    def get_height(self) -> float:
        return (self._height)

    def get_age(self) -> int:
        return (self._plant_age)


if __name__ == "__main__":
    plant: Plant = Plant("Rose", 15.0, 10, 0.8)

    print("=== Garden Security System ===")
    print("Plant created: ", end="")
    plant.show()
    print("\nHeigth updated: 25cm")
    plant.set_height(25.0)
    print("Age updated: 30 days", end="\n\n")
    plant.set_age(30)
    plant.set_height(-25)
    plant.set_age(-30)
    print("\nCurrent state: ", end="")
    plant.show()
