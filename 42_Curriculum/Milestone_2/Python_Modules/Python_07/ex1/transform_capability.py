from abc import ABC, abstractmethod
from ex0.creature import Creature


class TransformCapability(ABC):
    @abstractmethod
    def transform(self) -> str:
        pass

    @abstractmethod
    def revert(self) -> str:
        pass


class Shiftling(Creature, TransformCapability):
    def __init__(self, name: str, type: list[str]):
        super().__init__(name, type)
        self.attack_name = "Vine Whip"
        self.is_tranformed = False

    def attack(self):
        if self.is_tranformed:
            return f"{self.name} performs a boosted strike!"
        else:
            return f"{self.name} attacks normally"

    def transform(self) -> str:
        self.is_tranformed = True
        return f"{self.name} shift into a sharper form"

    def revert(self) -> str:
        self.is_tranformed = False
        return f"{self.name} returns to normal"


class Morphagon(Creature, TransformCapability):
    def __init__(self, name: str, type: list[str]):
        super().__init__(name, type)
        self.attack_name = "Vine Whip"
        self.is_tranformed = False

    def attack(self):
        if self.is_tranformed:
            return f"{self.name} unleashes a \
devastating morph strike!"
        else:
            return f"{self.name} attacks normally"

    def transform(self) -> str:
        self.is_tranformed = True
        return f"{self.name} morphs into a dragonic \
battle form"

    def revert(self) -> str:
        self.is_tranformed = False
        return f"{self.name} stabilizes its form"
