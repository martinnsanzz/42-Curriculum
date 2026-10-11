# Installed modules
import pytest
from pydantic import ValidationError

# Local modules
from main.models import HubMetadata, ConnectionMetadata, Zone

class TestHubClass:
    def test_invalid_metadata(self) -> None:
        with pytest.raises(ValidationError):
            HubMetadata(zone="Normal")

    def test_empty_metadata(self) -> None:
        meta = HubMetadata()
        assert meta.zone == Zone.NORMAL
        assert meta.color == None
        assert meta.max_drones == 1

    def test_valid_metadata(self) -> None:
        meta = HubMetadata(zone=Zone.NORMAL, color="Green", max_drones=2)
        assert meta.zone == Zone.NORMAL
        assert meta.color == "Green"
        assert meta.max_drones == 2

class TestConnectionClass:
    def test_invalid_metadata(self) -> None:
        with pytest.raises(ValidationError):
            ConnectionMetadata(max_link_capacity="two")

    def test_empty_metadata(self) -> None:
        meta = ConnectionMetadata()
        assert meta.max_link_capacity == 1

    def test_valid_metadata(self) -> None:
        meta = ConnectionMetadata(max_link_capacity=2)
        assert meta.max_link_capacity == 2