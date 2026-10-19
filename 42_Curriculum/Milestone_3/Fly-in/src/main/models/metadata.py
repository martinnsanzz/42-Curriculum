# Built-in modules
from typing import Optional
from enum import Enum

# Installed modules
from pydantic import BaseModel, Field, ConfigDict


class ZoneType(str, Enum):
    NORMAL = "normal"
    BLOCKED = "blocked"
    RESTRICTED = "restricted"
    PRIORITY = "priority"

class HubMetadata(BaseModel):
    model_config = ConfigDict(validate_by_name=True,
                              validate_by_alias=True)

    zone_type: ZoneType = Field(default=ZoneType.NORMAL, alias="zone")
    color: Optional[str] = Field(default=None)
    max_drones: int = Field(default=1, ge=1)

class ConnectionMetadata(BaseModel):
    max_link_capacity: int = Field(default=1, ge=1)
