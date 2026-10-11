# Installed modules
import pytest
from pydantic import ValidationError

# Local modules
from main.models import Hub, HubMetadata, Zone
from main.exceptions import MapError

def test_hub_metadata_is_required() -> None:
    with pytest.raises(ValidationError):
        Hub(line=1, name="a", x=0, y=0)

def test_invalid_hub_name() -> None:
    with pytest.raises(MapError, match="Illegal character found"):
        Hub(line=1, name="invalid-name", x=0, y=0, metadata=None)

def test_valid_hub_without_metadata() -> None:
    hub = Hub(line=1, name="a", x=0, y=0, metadata=None)
    assert hub.metadata is None

def test_valid_hub_with_metadata() -> None:
    meta = HubMetadata(zone=Zone.NORMAL, color="green", max_drones=1)
    hub = Hub(line=1, name="a", x=0, y=0, metadata=meta)
    assert hub.line == 1
    assert hub.name == "a"
    assert hub.x == 0
    assert hub.y == 0
    assert hub.metadata is not None
    assert hub.metadata.color == "green"
    assert hub.metadata.zone == Zone.NORMAL
    assert hub.metadata.max_drones == 1