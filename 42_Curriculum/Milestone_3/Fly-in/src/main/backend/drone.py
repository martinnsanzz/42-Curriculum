# Built-in modules
from typing import Optional
from enum import Enum, auto

# Local modules
from .zone import Zone

class DroneState(Enum):
    """Possible states a Drone can have during simulation"""
    WAITING = auto()
    MOVING = auto()
    FINISH = auto()


class Drone:
    def __init__(self, id: int, start_x: int,
                start_y: int, start_zone: str) -> None:
        self.id = id
        self.x = start_x
        self.y = start_y
        self.curr_zone = start_zone
        self.target_zone: Optional[str] = None
        self.state: DroneState = DroneState.WAITING
        self.turns_left: int = 0

    @property
    def connection_name(self) -> Optional[str]:
        if self.target_zone is None:
            return None
        return f"{self.curr_zone}-{self.target_zone}"

    def set_target(self, target: str) -> None:
        self.target_zone = target

    def set_state(self, new_state: DroneState) -> None:
        self.state = new_state


class Move:
    def __init__(self, id: int, zone: Optional[Zone] = None,
                 connection: Optional[str] = None) \
                 -> None:
        self.id = id
        self.zone = zone
        self.connection = connection