# Built-in modules
from typing import Self

# Installed modules
from pydantic import BaseModel, model_validator

# Local modules
from .hub import Hub
from .connection import Connection

class DroneMap(BaseModel):
    """Validated drone map: drone count, start/end hubs, hubs, connections.

    The model-level validators below run after field validation, in
    definition order. Any `ValueError` they raise is wrapped by Pydantic in
    a `ValidationError`, which `parse_map` converts to `MapError`.

    Attributes:
        nb_drones (int): Number of drones in the simulation.
        start (Hub): The start hub. Must be named `"start"`.
        end (Hub): The end hub. Must be named `"goal"` or
            `"impossible_goal"`.
        hubs (list[Hub]): Intermediate hubs, excluding `start` and `end`.
        connections (list[Connection]): Connections between hubs, matched
            by hub name.
    """
    nb_drones: int
    start: Hub
    end: Hub
    hubs: list[Hub]
    connections: list[Connection]

    @model_validator(mode="after")
    def modify_metadata(self) -> Self:
        """Set `max_drones` of the start and end hubs to `nb_drones`.

        Mutate the models in place. Any `max_drones` value given in the
        metadata of the start or end hub is overwritten. This
        lets both hubs hold every drone.

        Returns:
            Self: The model with the updated start and end metadata.
        """
        self.start.metadata.max_drones = self.nb_drones
        self.end.metadata.max_drones = self.nb_drones
        return self

    @model_validator(mode="after")
    def check_hub_names(self) -> Self:
        """Check the reserved names and the uniqueness of hub names.

        Require the start hub to be named `"start"` and the end hub to be
        named `"goal"` or `"impossible_goal"`. Reject intermediate hubs
        that use any of these reserved names, and reject duplicate names
        among the intermediate hubs.

        Returns:
            Self: The validated model, unchanged.

        Raises:
            ValueError: If the start hub name is not `"start"`.
            ValueError: If the end hub name is not an allowed end name.
            ValueError: If an intermediate hub uses a reserved start or
                end name.
            ValueError: If two intermediate hubs share a name.
        """
        start_names = ["start"]
        end_names = ["goal", "impossible_goal"]
        seen_names = []

        if self.start.name not in start_names:
                    raise ValueError("Start hub name must be 'start'.\n" \
                                   f"Current -> '{self.start.name}'")
        elif self.end.name not in end_names:
            raise ValueError(f"End hub name must be '{end_names[0]}' " \
                            f"or '{end_names[1]}'\n" \
                            f"Current -> '{self.end.name}'")
        for hub in self.hubs:
            if hub.name in start_names:
                raise ValueError("Non start_hub can't have name 'start'.\n" \
                               f"Check hub at line '{hub.line}'.")
            if hub.name in end_names:
                raise ValueError("Non end_hub can't have any of the following " \
                               f"names -> {end_names}.\n" \
                               f"Check hub at line '{hub.line}'.")
            if hub.name in seen_names:
                raise ValueError(f"Found hub with duplicate name '{hub.name}'.\n" \
                               f"Check hub at line '{hub.line}'.")
            seen_names.append(hub.name)
        return self

    @model_validator(mode="after")
    def check_hub_coordinates(self) -> Self:
        """Check that no two hubs share the same coordinates.

        Compare the `(x, y)` pairs of the start hub, the end hub and every
        intermediate hub.

        Returns:
            Self: The validated model, unchanged.

        Raises:
            ValueError: If the start and end hubs have the same
                coordinates.
            ValueError: If an intermediate hub has the same coordinates as
                the start hub or the end hub.
            ValueError: If two intermediate hubs have the same
                coordinates.
        """
        start_coords = [self.start.x, self.start.y]
        end_coords = [self.end.x, self.end.y]
        seen_cords = []

        if start_coords == end_coords:
            raise ValueError("Start and end coordinates can't be the same.")

        for hub in self.hubs:
            hub_cords = [hub.x, hub.y]

            if hub_cords == start_coords:
                raise ValueError(f"Hub '{hub.name}' at line '{hub.line}' can't " \
                               "have same coords as start_hub.")
            elif hub_cords == end_coords:
                raise ValueError(f"Hub '{hub.name}' at line '{hub.line}' can't " \
                               " have same coords as end_hub.")

            if hub_cords in seen_cords:
                raise ValueError(f"Hub '{hub.name}' at line '{hub.line}' contains "\
                               "duplicate coordinates.")
            seen_cords.append(hub_cords)
        return self

    @model_validator(mode="after")
    def check_connections_link(self) -> Self:
        """Check that every connection links two distinct, existing hubs.

        Compare connection names against the names of the start hub, the
        end hub and the intermediate hubs. Connections are treated as
        undirected when looking for duplicates (`a-b` equals `b-a`).

        Returns:
            Self: The validated model, unchanged.

        Raises:
            ValueError: If a connection links a hub to itself.
            ValueError: If a connection names a hub that does not exist.
            ValueError: If the same pair of hubs is connected more than
                once, in either direction.
        """
        hub_names = [hub.name for hub in (self.start, self.end, *self.hubs)]
        seen = {}

        for conn in self.connections:
            key = tuple(sorted((conn.name1, conn.name2)))

            if conn.name1 == conn.name2:
                raise ValueError(f"Connection at line '{conn.line}' " \
                                 "connects a hub to itself.")

            if conn.name1 not in hub_names:
                raise ValueError(f"Connection at line '{conn.line}' has a " \
                                 "non-existing hub name.\n" \
                                 f"- Invalid name -> {conn.name1}")
            elif conn.name2 not in hub_names:
                raise ValueError(f"Connection at line '{conn.line}' has a " \
                                 "non-existing hub name.\n" \
                                 f"- Invalid name -> {conn.name2}")
            if key in seen:
                first = seen[key]
                raise ValueError(f"Found duplicate connection '{conn.name1}-" \
                                 f"{conn.name2}' at line {conn.line}.\n" \
                                 f"- Same as line '{first.line}'")
            seen[key] = conn
        return self