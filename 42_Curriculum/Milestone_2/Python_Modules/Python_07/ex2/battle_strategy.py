from abc import ABC, abstractmethod
from typing import Any
from ex0.creature import Creature
import ex1


class BattleStrategy(ABC):
    @abstractmethod
    def is_valid(self, creature: Any) -> bool:
        pass

    @abstractmethod
    def act(self, creature: Any) -> None:
        pass


class CustomError(Exception):
    def __init__(self, msg: str):
        super().__init__(msg)


class NormalStrategy(BattleStrategy):
    strat_name = "Normal"

    def is_valid(self, creature: Any) -> bool:
        return isinstance(creature, Creature)

    def act(self, creature: Any) -> None:
        if not self.is_valid(creature):
            raise CustomError(f"Invalid Creature {creature.name} \
for this normal strategy")
        print(creature.attack())


class AggresiveStrategy(BattleStrategy):
    strat_name = "Aggressive"

    def is_valid(self, creature: Any) -> bool:
        return isinstance(creature, Creature) and \
               isinstance(creature, ex1.TransformCapability)

    def act(self, creature: Any) -> None:
        if not self.is_valid(creature):
            raise CustomError(f"Invalid Creature {creature.name} \
for this aggresive strategy")
        print(creature.transform())
        print(creature.attack())
        print(creature.revert())


class DefensiveStrategy(BattleStrategy):
    strat_name = "defensive"

    def is_valid(self, creature: Any) -> bool:
        return isinstance(creature, Creature) and \
               isinstance(creature, ex1.HealCapability)

    def act(self, creature: Any) -> None:
        if not self.is_valid(creature):
            raise CustomError(f"Invalid Creature {creature.name} \
for this defensive strategy")
        print(creature.attack())
        print(creature.heal())
