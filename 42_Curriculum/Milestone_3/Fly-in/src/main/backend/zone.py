# Built-in modules
from typing import Optional

# Local modules
from ..models.metadata import ZoneType

class Zone:
    """A simulation zone built from a hub, with a drone capacity counter.

    Args:
        name (str): Zone name.
        x (int): X coordinate.
        y (int): Y coordinate.
        zone_type (ZoneType): Zone category.
        color (Optional[str]): Display color name, `"rainbow"`, or `None`.
        max_drones (int): Maximum number of drones the zone can hold at
            once.
    """
    def __init__(self, name: str, x: int, y: int,
                 zone_type: ZoneType, color: Optional[str],
                 max_drones: int) -> None:
        self.name = name
        self.x = x
        self.y = y
        self.zone_type = zone_type
        self.color = color
        self.max_drones = max_drones
        self.current_capacity = max_drones

    def can_enter_zone(self) -> bool:
        if self.current_capacity == 0:
            return False
        return True
