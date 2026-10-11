# Installed modules
from pydantic import BaseModel, model_validator
from typing import Self

# Local modules
from .hub import Hub
from .connection import Connection
from ..exceptions import MapError

class DroneMap(BaseModel):
    nb_drones: int
    start: Hub
    end: Hub
    hubs: list[Hub]
    connections: list[Connection]

    @model_validator(mode="after")
    def check_start_end_name(self) -> Self:
        start_names = ["start"]
        end_names = ["goal", "impossible_goal"]

        if self.start.name not in start_names:
            raise MapError("Start hub name must be 'start'.\n" \
                           f"   Current -> '{self.start.name}'\n")

        if self.end.name not in end_names:
            raise MapError(f"End hub name must be '{end_names[0]}' " \
                           f"or '{end_names[1]}'\n" \
                           f"   Current -> '{self.end.name}'\n")
        return self