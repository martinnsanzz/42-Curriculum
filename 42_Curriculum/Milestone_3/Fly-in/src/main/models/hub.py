# Built-in modules
from typing import Optional

# Installed modules
from pydantic import BaseModel, model_validator

# Local modules
from .metadata import HubMetadata
from ..exceptions import MapError

class Hub(BaseModel):
    line: int
    name: str
    x: int
    y: int
    metadata: Optional[HubMetadata]

    @model_validator(mode="after")
    def check_valid_name(self):
        if "-" in self.name:
            raise MapError("Illegal character found on the hub at line " \
                           f"'{self.line}'.\n   " \
                            "Names can't contain '-' within the name.\n")
        return self