# Built-in modules
from typing import Optional
from enum import Enum, auto

# Local modules
from ..models import Hub


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
        start_hub (str): Name of the hub the drone starts in.

    Attributes:
        id (int): Drone identifier.
        x (int): Current X coordinate.
        y (int): Current Y coordinate.
        curr_hub (str): Name of the hub the drone is in.
        target_hub (Optional[str]): Name of the hub the drone is
            heading to, or `None` if it has no target.
        state (DroneState): Current state. Starts as
            `DroneState.WAITING`.
        turns_left (int): Turns remaining for the current action. Starts
            at 0. This is used when entering hubs that cost more than 1
            move such as `restricted`.
    """
    def __init__(self, id: int, start_x: int,
                 start_y: int, start_hub: str) -> None:
        self.id = id
        self.x = start_x
        self.y = start_y
        self.curr_hub = start_hub
        self.target_hub: Optional[str] = None
        self.state: DroneState = DroneState.WAITING
        self.turns_left: int = 0

    @property
    def connection_name(self) -> Optional[str]:
        """Name of the connection from the current hub to the target.

        Returns:
            Optional[str]: `"<curr_hub>-<target_hub>"`, or `None` if
                there is no target hub.
        """
        if self.target_hub is None:
            return None
        return f"{self.curr_hub}-{self.target_hub}"

    def set_target(self, target: str) -> None:
        """Set the hub the drone is heading to."""
        self.target_hub = target

    def set_state(self, new_state: DroneState) -> None:
        """Set the drone state."""
        self.state = new_state


class Move:
    """A single drone action in one simulation turn.

    A move describes a drone leaving one hub toward another. If the
    connection takes several turns, the move is recorded twice: once on
    the turn the drone enters the connection (``connection`` is set) and
    once on the turn it arrives (``connection`` is None).

    Args:
        id: Identifier of the drone that moved.
        from_hub: Name of the hub the drone left.
        to_hub: Name of the hub the drone is heading to (in transit) or
            has arrived in.
        connection: Name of the connection the drone is travelling on if
            it is still in transit, otherwise None.
    """
    def __init__(self, id: int, from_hub: str, to_hub: Hub,
                 connection: Optional[str] = None) -> None:
        self.id = id
        self.from_hub = from_hub
        self.to_hub = to_hub
        self.connection = connection

    @property
    def in_transit(self) -> bool:
        """Whether the drone is still on a multi-turn connection."""
        return self.connection is not None
