from abc import ABC, abstractmethod

from .creature import Creature, Flameling, Aquabub, Torragon, Pyrodon


class CreatureFactory(ABC):
    @abstractmethod
    def create_base(self) -> Creature:
        pass

    @abstractmethod
    def create_evolved(self) -> Creature:
        pass


class FlameFactory(CreatureFactory):
    def create_base(self) -> Creature:
        flameling = Flameling("flameling", ["fire"])
        return flameling

    def create_evolved(self) -> Creature:
        pyrodon = Pyrodon("pyrodon", ["fire", "flying"])
        return pyrodon


class AquaFactory(CreatureFactory):
    def create_base(self) -> Creature:
        aquabub = Aquabub("aquabub", ["water"])
        return aquabub

    def create_evolved(self) -> Creature:
        torragon = Torragon("torragon", ["water"])
        return torragon
