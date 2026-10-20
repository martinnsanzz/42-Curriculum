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
    """A drone moving through the map, tracked by position and state.

    Args:
        id (int): Drone identifier.
        start_x (int): Initial X coordinate.
        start_y (int): Initial Y coordinate.
        start_zone (str): Name of the zone the drone starts in.

    Attributes:
        id (int): Drone identifier.
        x (int): Current X coordinate.
        y (int): Current Y coordinate.
        curr_zone (str): Name of the zone the drone is in.
        target_zone (Optional[str]): Name of the zone the drone is
            heading to, or `None` if it has no target.
        state (DroneState): Current state. Starts as
            `DroneState.WAITING`.
        turns_left (int): Turns remaining for the current action. Starts
            at 0. This is used when entering zones that cost more than 1
            move such as `restricted`.
    """
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
        """Name of the connection from the current zone to the target.

        Returns:
            Optional[str]: `"<curr_zone>-<target_zone>"`, or `None` if
                there is no target zone.
        """
        if self.target_zone is None:
            return None
        return f"{self.curr_zone}-{self.target_zone}"

    def set_target(self, target: str) -> None:
        """Set the zone the drone is heading to."""
        self.target_zone = target

    def set_state(self, new_state: DroneState) -> None:
        """Set the drone state."""
        self.state = new_state


class Move:
    """A single drone move in one simulation turn, ready to be printed.

    A move is expected to carry either a zone (the drone arrived in a
    zone) or a connection (the drone is in transit).

    Args:
        id (int): Identifier of the drone that moved.
        zone (Optional[Zone]): The zone the drone moved into, if any.
        connection (Optional[str]): The connection name the drone is on,
            if any.
    """
    def __init__(self, id: int, zone: Optional[Zone] = None,
                 connection: Optional[str] = None) \
                 -> None:
        self.id = id
        self.zone = zone
        self.connection = connection