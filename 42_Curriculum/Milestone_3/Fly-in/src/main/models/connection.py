# Built-in modules
from typing import Optional

# Installed modules
from pydantic import BaseModel

# Local modules
from .metadata import ConnectionMetadata

class Connection(BaseModel):
    line: int
    name1: str
    name2: str
    metadata: Optional[ConnectionMetadata]