# Built-in modules
from enum import Enum, auto
# Local modules

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
