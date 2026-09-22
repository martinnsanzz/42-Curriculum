from abc import ABC, abstractmethod
from ex0.creature import Creature


class HealCapability(ABC):
    @abstractmethod
    def heal(self) -> str:
        pass


class Sproutling(Creature, HealCapability):
    def __init__(self, name: str, type: list[str]):
        super().__init__(name, type)
        self.attack_name = "Vine Whip"

    def attack(self):
        return f"{self.name} uses {self.attack_name}!"

    def heal(self) -> str:
        return f"{self.name} heals itself \
for a small amount"


class Bloomelle(Creature, HealCapability):
    def __init__(self, name: str, type: list[str]):
        super().__init__(name, type)
        self.attack_name = "Petal Dance"

    def attack(self):
        return f"{self.name} uses {self.attack_name}!"

    def heal(self) -> str:
        return f"{self.name} heals itself and others \
for a large amount"
