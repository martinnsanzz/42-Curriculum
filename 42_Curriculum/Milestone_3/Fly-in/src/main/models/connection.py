# Built-in modules
from typing import Optional

# Installed modules
from pydantic import BaseModel, Field

# Local modules
from .metadata import ConnectionMetadata

class Connection(BaseModel):
    """A connection (edge) between two hubs, parsed from a connection line.

    Attributes:
        line (int): Line number in the source file, used in error
            messages.
        name1 (str): Name of the first hub.
        name2 (str): Name of the second hub.
        metadata (ConnectionMetadata): Optional connection metadata.
            Defaults to a `ConnectionMetadata` with all default values.
    """
    line: int
    name1: str
    name2: str
    metadata: ConnectionMetadata = Field(default_factory=ConnectionMetadata)