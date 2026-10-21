# Installed modules
import pytest
from pydantic import ValidationError

# Local modules
from src.main.models import HubMetadata, ConnectionMetadata, HubType


class TestHubClass:
    def test_invalid_metadata(self) -> None:
        with pytest.raises(ValidationError):
            HubMetadata(hub_type="Normal")  # noqa # type: ignore[call-arg]  # intentional: testing bad input

    def test_empty_metadata(self) -> None:
        meta = HubMetadata()
        assert meta.hub_type == HubType.NORMAL
        assert meta.color is None
        assert meta.max_drones == 1

    def test_valid_metadata(self) -> None:
        meta = HubMetadata(zone=HubType.NORMAL, color="Green", max_drones=2)
        assert meta.hub_type == HubType.NORMAL
        assert meta.color == "Green"
        assert meta.max_drones == 2


class TestConnectionClass:
    def test_invalid_metadata(self) -> None:
        with pytest.raises(ValidationError):
            ConnectionMetadata(max_link_capacity="two")  # noqa # type: ignore[call-arg]  # intentional: testing bad input

    def test_empty_metadata(self) -> None:
        meta = ConnectionMetadata()
        assert meta.max_link_capacity == 1

    def test_valid_metadata(self) -> None:
        meta = ConnectionMetadata(max_link_capacity=2)
        assert meta.max_link_capacity == 2
