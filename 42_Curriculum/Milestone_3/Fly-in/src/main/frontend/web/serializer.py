# Built-in modules
from typing import Any

# Local modules
from ...models import DroneMap, Hub


def serialize_map(drone_map: DroneMap) -> dict[str, Any]:
    """Serialize a DroneMap into a dictionary of hubs and connections.

    Hubs are emitted in a fixed order: the start hub first, then every
    intermediate hub in ``drone_map.hubs`` order, and the end hub last.
    Each hub is tagged with its role.

    Args:
        drone_map: The map to serialize. Must expose ``start``, ``hubs``,
            ``end`` and ``connections``.

    Returns:
        A dictionary with two keys:

        * ``"hubs"``: List of hub dictionaries, each containing
          ``name``, ``x``, ``y``, ``color`` and ``role`` (``"start"``,
          ``"hub"`` or ``"end"``).
        * ``"connections"``: List of dictionaries of the form
          ``{"from": name1, "to": name2}``, one per connection, in the
          order of ``drone_map.connections``.
    """
    hubs = [_hub_to_dict(drone_map.start, "start")]

    for hub in drone_map.hubs:
        hubs.append(_hub_to_dict(hub, "hub"))

    hubs.append(_hub_to_dict(drone_map.end, "end"))

    connections = []
    for conn in drone_map.connections:
        connections.append({"from": conn.name1, "to": conn.name2})

    return {"hubs": hubs, "connections": connections}

def _hub_to_dict(hub: Hub, role: str) -> dict[str, Any]:
    """Converta a hub into a dictionary"""
    return {"name": hub.name, "x": hub.x, "y": hub.y,
            "color": hub.metadata.color, "role": role}