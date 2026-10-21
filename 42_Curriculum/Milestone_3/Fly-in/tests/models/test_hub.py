# Installed modules
import pytest

# Local modules
from src.main.models import Hub, HubMetadata, HubType


def test_invalid_hub_name() -> None:
    with pytest.raises(ValueError, match="Illegal character found"):
        Hub(line=1, name="invalid-name", x=0, y=0)


def test_valid_hub_with_default_metadata() -> None:
    hub = Hub(line=1, name="a", x=0, y=0)
    assert hub.line == 1
    assert hub.name == "a"
    assert hub.x == 0
    assert hub.y == 0
    assert hub.metadata is not None
    assert hub.metadata.color is None
    assert hub.metadata.hub_type == HubType.NORMAL
    assert hub.metadata.max_drones == 1


def test_valid_hub_with_metadata() -> None:
    meta = HubMetadata(zone=HubType.NORMAL, color="green", max_drones=1)
    hub = Hub(line=1, name="a", x=0, y=0, metadata=meta)
    assert hub.line == 1
    assert hub.name == "a"
    assert hub.x == 0
    assert hub.y == 0
    assert hub.metadata is not None
    assert hub.metadata.color == "green"
    assert hub.metadata.hub_type == HubType.NORMAL
    assert hub.metadata.max_drones == 1
