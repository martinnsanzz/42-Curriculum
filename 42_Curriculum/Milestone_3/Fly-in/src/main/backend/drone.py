# Built-in modules
from typing import Optional
from enum import Enum, auto

# Local modules
from .zone import Zone

class DroneState(Enum):
    WAITING = auto()
    MOVING = auto()
    FINISH = auto()


class Drone:
    def __init__(self, id: int, start_x: int,
                start_y: int, start_pos: str) -> None:
        self.id = id
        self.x = start_x
        self.y = start_y
        self.curr_zone = start_pos
        self.state: DroneState = DroneState.WAITING

    def set_state(self, new_state: DroneState) -> None:
        self.state = new_state

    def set_curr_zone(self, zone: str) -> None:
        self.curr_zone = zone

    def get_curr_zone(self) -> str:
        return self.curr_zone

    def get_state(self) -> DroneState:
        return self.state

class Move:
    def __init__(self, id: int, zone: Optional[Zone] = None,
                 connection: Optional[str] = None) -> None:
        self.id = id
        self.zone = zone
        self.connection = connection