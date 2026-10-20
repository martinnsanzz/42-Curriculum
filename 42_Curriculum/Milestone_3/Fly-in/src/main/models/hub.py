# Built-in modules
from typing import Optional, Self

# Installed modules
from pydantic import BaseModel, model_validator, Field

# Local modules
from .metadata import HubMetadata

class Hub(BaseModel):
    """A hub (node) of the drone map, parsed from a hub line.

    Attributes:
        line (int): Line number in the source file, used in error
            messages.
        name (str): Hub name. Must not contain `'-'`.
        x (int): X coordinate.
        y (int): Y coordinate.
        metadata (HubMetadata): Optional hub metadata. Defaults to a
            `HubMetadata` with all default values.
    """
    line: int
    name: str
    x: int
    y: int
    metadata: HubMetadata = Field(default_factory=HubMetadata)

    @model_validator(mode="after")
    def check_valid_name(self) -> Self:
        if "-" in self.name:
            raise ValueError("Illegal character found on the hub at line " \
                           f"'{self.line}'.\n" \
                            "Names can't contain '-' within the name.")
        return self
