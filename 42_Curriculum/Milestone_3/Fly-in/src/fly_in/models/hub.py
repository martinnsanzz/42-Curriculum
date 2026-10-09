# Built-in modules
from typing import Optional

# Installed modules
from pydantic import BaseModel

# Local modules
from .metadata import HubMetadata

class Hub(BaseModel):
    line: int
    name: str
    x: int
    y: int
    metadata: Optional[HubMetadata]
