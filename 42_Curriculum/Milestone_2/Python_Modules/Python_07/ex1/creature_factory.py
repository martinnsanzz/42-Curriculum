from ex0 import CreatureFactory
from ex0.creature import Creature
from .heal_capability import Sproutling, Bloomelle
from .transform_capability import Shiftling, Morphagon


class HealingCreatureFactory(CreatureFactory):
    def create_base(self) -> Creature:
        sproutling = Sproutling("sproutling", ["grass"])
        return sproutling

    def create_evolved(self) -> Creature:
        bloomelle = Bloomelle("bloomelle", ["grass", "fairy"])
        return bloomelle


class TransformCreatureFactory(CreatureFactory):
    def create_base(self) -> Creature:
        shiftling = Shiftling("shiftling", ["normal"])
        return shiftling

    def create_evolved(self) -> Creature:
        morphagon = Morphagon("morphagon", ["normal", "dragon"])
        return morphagon
