# Installed modules
from pydantic import BaseModel

# Local modules
from .hub import Hub
from .connection import Connection

class DroneMap(BaseModel):
    nb_drones: int
    start: Hub
    end: Hub
    hubs: list[Hub]
    connections: list[Connection]