# Built-in modules
from typing import Optional

# Local modules
from ..models.metadata import ZoneType

class Zone:
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
