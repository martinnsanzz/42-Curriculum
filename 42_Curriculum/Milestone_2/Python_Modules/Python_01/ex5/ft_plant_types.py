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


class Flower(Plant):
    def __init__(self, name: str, height: float, plant_age: int,
                 growth_rate: float, color: str) -> None:
        super().__init__(name, height, plant_age, growth_rate)
        self.color = color
        self.has_bloom = False

    def show(self) -> None:
        super().show()
        print(f" Color: {self.color}")
        if not self.has_bloom:
            print(f" {self.name} has not bloomed yet")
        else:
            print(f" {self.name} is blooming beautifully!")

    def bloom(self) -> None:
        if not self.has_bloom:
            self.show()
            self.has_bloom = True
        else:
            print(f" {self.name} has already bloomed...")


class Tree(Plant):
    def __init__(self, name: str, height: float, plant_age: int,
                 growth_rate: float, trunk_diameter: float) -> None:
        super().__init__(name, height, plant_age, growth_rate)
        self.trunk_diameter = trunk_diameter

    def show(self) -> None:
        super().show()
        print(f" Trunk diameter: {self.trunk_diameter}cm")

    def produce_shade(self) -> None:
        print(f"Tree {self.name} now produces a shade ", end="")
        print(f"of {self._height}cm ", end="")
        print(f"long and {self.trunk_diameter}cm wide.")


class Vegetable(Plant):
    def __init__(self, name: str, height: float, plant_age: int,
                 growth_rate: float, harvest_season: str,
                 nutritional_value: int) -> None:
        super().__init__(name, height, plant_age, growth_rate)
        self.harvest_season = harvest_season
        self.nutritional_value = nutritional_value

    def show(self) -> None:
        super().show()
        print(f" Harvest season: {self.harvest_season}")
        print(f" Nutritional value: {self.nutritional_value}")

    def age(self) -> None:
        super().age()

    def grow(self, days: int) -> None:
        self.nutritional_value = days
        for x in range(days):
            self.age()
        self.show()


if __name__ == "__main__":
    flower: Flower = Flower("Rose", 15.0, 10, 0.0, "red")
    tree: Tree = Tree("Oak", 200.0, 365, 0.0, 5.0)
    vegetable: Vegetable = Vegetable("Tomato", 5.0, 10, 2.1, "April", 0)

    print("=== Garden Plant Types ===")
    print("=== Flower")
    flower.show()
    print(f"[asking the {flower.name.lower()} to bloom]")
    flower.bloom()
    print("\n=== Tree")
    tree.show()
    print(f"[asking the {tree.name.lower()} to produce shade]")
    tree.produce_shade()
    print("\n=== Vegetable")
    vegetable.show()
    print(f"[make {vegetable.name.lower()} grow and age for ", end="")
    print("20 days]")
    vegetable.grow(20)
