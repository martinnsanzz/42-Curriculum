# Built-in modules
from typing import Self

# Installed modules
from pydantic import BaseModel, model_validator

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
    def modify_metadata(self) -> Self:
        self.start.metadata.max_drones = self.nb_drones
        self.end.metadata.max_drones = self.nb_drones
        return self

    @model_validator(mode="after")
    def check_hub_names(self) -> Self:
        start_names = ["start"]
        end_names = ["goal", "impossible_goal"]
        seen_names = []

        if self.start.name not in start_names:
                    raise ValueError("Start hub name must be 'start'.\n" \
                                   f"   Current -> '{self.start.name}'\n")
        elif self.end.name not in end_names:
            raise ValueError(f"End hub name must be '{end_names[0]}' " \
                            f"or '{end_names[1]}'\n" \
                            f"   Current -> '{self.end.name}'\n")
        for hub in self.hubs:
            if hub.name in start_names:
                raise ValueError(f"Non start_hub can't have name 'start'.\n" \
                               f"   Check hub at line '{hub.line}'.\n")
            if hub.name in end_names:
                raise ValueError(f"Non end_hub can't have any of the following " \
                               f"names -> {end_names}.\n" \
                               f"   Check hub at line '{hub.line}'.\n")
            if hub.name in seen_names:
                raise ValueError(f"Found hub with duplicate name '{hub.name}'.\n" \
                               f"   Check hub at line '{hub.line}'.\n")
            seen_names.append(hub.name)
        return self

    @model_validator(mode="after")
    def check_hub_coordinates(self) -> Self:
        start_coords = [self.start.x, self.start.y]
        end_coords = [self.end.x, self.end.y]
        seen_cords = []

        if start_coords == end_coords:
            raise ValueError("Start and end coordinates can't be the same !!\n")

        for hub in self.hubs:
            hub_cords = [hub.x, hub.y]

            if hub_cords == start_coords:
                raise ValueError(f"Hub '{hub.name}' at line '{hub.line}' can't " \
                               "have same coords as start_hub.\n")
            elif hub_cords == end_coords:
                raise ValueError(f"Hub '{hub.name}' at line '{hub.line}' can't " \
                               " have same coords as end_hub.\n")

            if hub_cords in seen_cords:
                raise ValueError(f"Hub '{hub.name}' at line '{hub.line}' contains "\
                               "duplicate coordinates.\n")
            seen_cords.append(hub_cords)
        return self

    @model_validator(mode="after")
    def check_connections_link(self) -> Self:
        hub_names = [hub.name for hub in (self.start, self.end, *self.hubs)]
        seen = {}

        for conn in self.connections:
            key = tuple(sorted((conn.name1, conn.name2)))

            if conn.name1 not in hub_names:
                raise ValueError(f"Connection at line '{conn.line}' has a " \
                                 f"non-existing hub name.\n" \
                                 f" - Invalid name -> {conn.name1}\n")
            elif conn.name2 not in hub_names:
                raise ValueError(f"Connection at line '{conn.line}' has a " \
                                 f"non-existing hub name.\n" \
                                 f" - Invalid name -> {conn.name2}\n")
            if key in seen:
                first = seen[key]
                raise ValueError(f"Found dupplicate connection '{conn.name1}-" \
                                 f"{conn.name2}' at line {conn.line}.\n" \
                                 f"     - Same as line '{first.line}'\n")
            seen[key] = conn
        return self