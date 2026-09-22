# Built-in module
from enum import Enum, auto
from typing import Optional


class State(Enum):
    """UI Actions. Each constant is a state."""
    QUIT = auto()
    START_WIN = auto()
    GEN_MAZE = auto()
    CHANGE_COLOR = auto()
    SOLVE = auto()

    @staticmethod
    def handle_input(key: str, state: Optional['State']) -> Optional['State']:
        """Map an uppercase input key to its corresponding state action."""
        actions = {
            "G": State.GEN_MAZE if state is State.START_WIN else None,
            "R": State.GEN_MAZE if state is not State.START_WIN else None,
            "C": State.CHANGE_COLOR,
            "Q": State.QUIT,
            "S": State.SOLVE}
        return actions.get(key.upper(), None)
