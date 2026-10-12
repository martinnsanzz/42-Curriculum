# Built-in modules
from typing import Optional, Self

# Installed modules
from pydantic import BaseModel, model_validator, Field

# Local modules
from .metadata import HubMetadata

class Hub(BaseModel):
    line: int
    name: str
    x: int
    y: int
    metadata: HubMetadata = Field(default_factory=HubMetadata)

    @model_validator(mode="after")
    def check_valid_name(self) -> Self:
        if "-" in self.name:
            raise ValueError("Illegal character found on the hub at line " \
                           f"'{self.line}'.\n   " \
                            "Names can't contain '-' within the name.\n")
        return self
