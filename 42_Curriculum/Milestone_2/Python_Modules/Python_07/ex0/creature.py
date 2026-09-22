from abc import ABC, abstractmethod


class Creature(ABC):
    def __init__(self, name: str, type: list[str]) -> None:
        self.name = name.capitalize()
        self.type = [t.capitalize() for t in type]

    def describe(self) -> str:
        return f"{self.name} is a \
{"/".join(self.type)} type Creature"

    @abstractmethod
    def attack(self) -> str:
        pass


class Flameling(Creature):
    def __init__(self, name: str, type: list[str]) -> None:
        super().__init__(name, type)
        self.attack_name = "Ember"

    def attack(self) -> str:
        return f"{self.name} uses {self.attack_name}!"


class Pyrodon(Creature):
    def __init__(self, name: str, type: list[str]) -> None:
        super().__init__(name, type)
        self.attack_name = "Flamethrower"

    def attack(self) -> str:
        return f"{self.name} uses {self.attack_name}!"


class Aquabub(Creature):
    def __init__(self, name: str, type: list[str]) -> None:
        super().__init__(name, type)
        self.attack_name = "Water Gun"

    def attack(self) -> str:
        return f"{self.name} uses {self.attack_name}!"


class Torragon(Creature):
    def __init__(self, name: str, type: list[str]) -> None:
        super().__init__(name, type)
        self.attack_name = "Hydro Pump"

    def attack(self) -> str:
        return f"{self.name} uses {self.attack_name}!"
