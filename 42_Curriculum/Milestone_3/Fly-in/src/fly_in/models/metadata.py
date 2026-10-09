# Built-in modules
from typing import Optional
from enum import Enum

# Installed modules
from pydantic import BaseModel, Field


class Zone(str, Enum):
    NORMAL = "normal"
    BLOCKED = "blocked"
    RESTRICTED = "restricted"
    PRIORITY = "priority"

class HubMetadata(BaseModel):
    zone: Zone = Zone.NORMAL
    color: Optional[str] = None
    max_drones: int = Field(default=1, ge=1)

class ConnectionMetadata(BaseModel):
    max_link_capacity: int = Field(default=1, ge=1)
