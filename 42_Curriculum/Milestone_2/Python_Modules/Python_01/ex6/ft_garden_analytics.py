#!usr/bin/env python3

class Plant:
    def __init__(self, name: str, height: float, plant_age: int,
                 growth_rate: float) -> None:
        self.name = name
        self._height = (height, 15.0)[height < 0]
        self._plant_age = (plant_age, 10)[plant_age < 0]
        self.growth_rate = growth_rate
        self.week_growth = 0.0
        self.stats = self.Stats(0, 0, 0)

        if (height < 0):
            print("Height can't be negative")
        if (plant_age < 0):
            print("Age can't be negative")

    def show(self) -> None:
        self.stats.show_count += 1
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
        self.stats.grow_count += 1
        if amount > 0:
            self._height = amount
        else:
            print(f"{self.name}: Error, height can't be negative")
            print("Height update rejected")

    def set_age(self, amount: int):
        self.stats.age_count += 1
        if amount > 0:
            self._plant_age = amount
        else:
            print(f"{self.name}: Error, age can't be negative")
            print("Age update rejected")

    def get_height(self) -> float:
        return (self._height)

    def get_age(self) -> int:
        return (self._plant_age)

    class Stats:
        def __init__(self, grow_count: int, age_count: int,
                     show_count: int) -> None:
            self.grow_count = grow_count
            self.age_count = age_count
            self.show_count = show_count

        def plants_stats(self) -> None:
            print(f"Stats: {self.grow_count} grow,", end=" ")
            print(f"{self.age_count} age,", end=" ")
            print(f"{self.show_count} show")

    @staticmethod
    def check_days(days: int) -> None:
        print(f"Is {days} days more than a year? -> ", end="")
        print(("False", "True")[days > 365])

    @classmethod
    def anonymous(cls) -> 'Plant':
        return cls("Unknown plant", 0.0, 0, 0.0)


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
            self.has_bloom = True
        else:
            print(f" {self.name} has already bloomed...")


class Tree(Plant):
    def __init__(self, name: str, height: float, plant_age: int,
                 growth_rate: float, trunk_diameter: float) -> None:
        super().__init__(name, height, plant_age, growth_rate)
        self.trunk_diameter = trunk_diameter
        self.stats : Tree.Stats = self.Stats(0, 0, 0, 0)

    def show(self) -> None:
        super().show()
        print(f" Trunk diameter: {self.trunk_diameter}cm")

    def produce_shade(self) -> None:
        self.stats.shade_count += 1
        print(f"Tree {self.name} now produces a shade ", end="")
        print(f"of {self._height}cm ", end="")
        print(f"long and {self.trunk_diameter}cm wide.")

    class Stats(Plant.Stats):
        def __init__(self, grow_count: int, age_count: int, show_count: int,
                     shade_count: int = 0) -> None:
            super().__init__(grow_count, age_count, show_count)
            self.shade_count = shade_count

        def plants_stats(self) -> None:
            super().plants_stats()
            print(f"{self.shade_count} shade")


class Seed(Flower):
    def __init__(self, name: str, height: float, plant_age: int,
                 growth_rate: float, color: str) -> None:
        super().__init__(name, height, plant_age, growth_rate, color)
        self.seeds_num = 0

    def show(self) -> None:
        super().show()
        print(f" Seeds: {self.seeds_num}")

    def bloom(self) -> None:
        self.seeds_num = 42
        super().bloom()


def plants_stats(plant: Plant) -> None:
    plant.stats.plants_stats()


if __name__ == "__main__":
    unknown: Plant = Plant.anonymous()
    flower: Flower = Flower("Rose", 15.0, 10, 0.0, "red")
    tree: Tree = Tree("Oak", 200.0, 365, 0.0, 5.0)
    seed: Seed = Seed("Sunflower", 80.0, 45, 0.3, "yellow")

    print("=== Garden statistics ===")
    print("=== Check year-old")
    unknown.check_days(30)
    unknown.check_days(400)

    print("\n=== Flower")
    flower.show()
    print(f"[statistics for {flower.name}]")
    plants_stats(flower)
    print(f"[asking the {flower.name.lower()} to grow and bloom]")
    flower.set_height(flower.get_height() + 8.0)
    flower.bloom()
    flower.show()
    print(f"[statistics for {flower.name}]")
    plants_stats(flower)

    print("\n=== Tree")
    tree.show()
    print(f"[statistics for {tree.name}]")
    plants_stats(tree)
    print(f"[asking the {tree.name.lower()} to produce shade]")
    tree.produce_shade()
    print(f"[statistics for {tree.name}]")
    plants_stats(tree)

    print("\n=== Seed")
    seed.show()
    print("[make sunflower grow, age, and bloom]")
    seed.set_age(seed.get_age() + 20)
    seed.set_height(seed.get_height() + 30.0)
    seed.bloom()
    seed.show()
    print(f"[statistics for {seed.name}]")
    plants_stats(seed)

    print("\n=== Anonymous")
    unknown.show()
    print(f"[statistics for {unknown.name}]")
    plants_stats(unknown)
