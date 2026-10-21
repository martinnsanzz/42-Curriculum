# Built-in modules
from typing import Optional
from enum import Enum

# Installed modules
from pydantic import BaseModel, Field, ConfigDict


class HubType(str, Enum):
    """Hub category of a hub."""
    NORMAL = "normal"
    BLOCKED = "blocked"
    RESTRICTED = "restricted"
    PRIORITY = "priority"


class HubMetadata(BaseModel):
    """Optional metadata attached to a hub line.

    Fields can be populated by field name or by alias (`validate_by_name`
    and `validate_by_alias` are both enabled).

    Attributes:
        hub_type (HubType): Hub category of the hub. Alias: `hub`.
            Defaults to `Hub.NORMAL`.
        color (Optional[str]): Display color. Any string is accepted, no
            format is validated. Defaults to `None`.
        max_drones (int): Maximum number of drones for the hub. Must be
            at least 1. Defaults to 1. [INFERRED]
    """
    model_config = ConfigDict(validate_by_name=True,
                              validate_by_alias=True)

    hub_type: HubType = Field(default=HubType.NORMAL, alias="zone")
    color: Optional[str] = Field(default=None)
    max_drones: int = Field(default=1, ge=1)


class ConnectionMetadata(BaseModel):
    """Optional metadata attached to a connection line.

    Attributes:
        max_link_capacity (int): Maximum number of drones the link can
            carry. Must be at least 1. Defaults to 1. [INFERRED]
    """
    max_link_capacity: int = Field(default=1, ge=1)
