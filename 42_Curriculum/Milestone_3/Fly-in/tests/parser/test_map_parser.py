# Built-in modules
from pathlib import Path
from typing import Callable

# Installed modules
import pytest
from pydantic import ValidationError

# Local modules
from main.parser.loader import RawLine
from main.parser.map_parser import parse_map, parse_connection, parse_hub, \
                                   parse_meta, parse_nb_drones
from main.models import HubMetadata, ConnectionMetadata, Zone
from main.exceptions import MapError

class TestParseMap():
    HEADER = "nb_drones: 2"
    START = "start_hub: start 0 0"
    END = "end_hub: goal 2 0"
    HUB = "hub: mid 1 0"
    CONNS = ["connection: start-mid", "connection: mid-goal"]
    VALID = [HEADER, START, END, HUB, *CONNS]


@pytest.fixture
def make_map(tmp_path: Path) -> Callable[..., Path]:
    def _make(*lines: str) -> Path:
        path = tmp_path / "map.txt"
        path.write_text("\n".join(lines) + "\n")
        return path
    return _make

class TestParseNbDrones():
    def test_negative_nb_drones(self) -> None:
        raw_line = RawLine(line=1, key="nb_drones", value="-1")
        with pytest.raises(MapError):
            parse_nb_drones(raw_line)

    def test_0_nb_drones(self) -> None:
        raw_line = RawLine(line=1, key="nb_drones", value="0")
        with pytest.raises(MapError):
            parse_nb_drones(raw_line)

    def test_invalid_nb_drones(self) -> None:
        raw_line = RawLine(line=1, key="nb_drones", value="a")
        with pytest.raises(MapError):
            parse_nb_drones(raw_line)

    def test_valid_nb_drones(self) -> None:
        raw_line = RawLine(line=1, key="nb_drones", value="5")
        nb_drones = parse_nb_drones(raw_line)
        assert nb_drones == 5

class TestParseConnection():
    def test_invalid_connection(self) -> None:
        raw_line = RawLine(line=1, key="connection", value="name1 name2")
        with pytest.raises(MapError, match="Incorrect connection format"):
            parse_connection(raw_line)

    def test_valid_connection_without_metadata(self) -> None:
        raw_line = RawLine(line=1, key="conn", value="name1-name2")
        conn = parse_connection(raw_line)
        assert conn.line == 1
        assert conn.name1 == "name1"
        assert conn.name2 == "name2"
        assert conn.metadata is not None
        assert conn.metadata.max_link_capacity == 1

    def test_valid_connection(self) -> None:
        raw_line = RawLine(line=1, key="connection",
                           value="name1-name2 [max_link_capacity=2]")
        conn = parse_connection(raw_line)
        assert conn.line == 1
        assert conn.name1 == "name1"
        assert conn.name2 == "name2"
        assert conn.metadata is not None

class TestParseHub():
    def test_invalid_hub(self) -> None:
        raw_line = RawLine(line=1, key="hub", value="name x 0 color=green")
        with pytest.raises(MapError, match="Incorrect hub format"):
            parse_hub(raw_line)

    def test_valid_hub_without_metadata(self) -> None:
        raw_line = RawLine(line=1, key="hub", value="name 0 0")
        hub = parse_hub(raw_line)
        assert hub.line == 1
        assert hub.name == "name"
        assert hub.x == 0
        assert hub.y == 0
        assert hub.metadata is not None

    def test_valid_hub(self) -> None:
        raw_line = RawLine(line=1, key="hub", value="name 0 0 [color=green]")
        hub = parse_hub(raw_line)
        assert hub.line == 1
        assert hub.name == "name"
        assert hub.x == 0
        assert hub.y == 0
        assert hub.metadata is not None

class TestParseMeta():
    def test_none_meta(self) -> None:
        raw_line = RawLine(line=1, key="hub", value="name 0 0")
        meta = parse_meta(raw_line, None, HubMetadata)
        assert meta == HubMetadata()

    def test_empty_meta(self) -> None:
        raw_line = RawLine(line=1, key="hub", value="name 0 0 []")
        with pytest.raises(MapError, match="Found metadata field empty"):
            parse_meta(raw_line, "[]", HubMetadata)

    def test_invalid_meta_format(self) -> None:
        raw_line = RawLine(line=1, key="hub", value="name 0 0 [color green]")
        with pytest.raises(MapError, match="Incorrect metadata format"):
            parse_meta(raw_line, "color green", HubMetadata)

    def test_repeated_meta(self) -> None:
        raw_line = RawLine(line=1, key="hub",
                           value="name 0 0 [color=green color=red]")
        with pytest.raises(MapError, match="Found repeated metadata key"):
            parse_meta(raw_line, "color=green color=red", HubMetadata)

    def test_invalid_hub_meta(self) -> None:
        raw_line = RawLine(line=1, key="hub",
                           value="name 0 0 [max_link_capacity=2]")
        with pytest.raises(MapError, match="Invalid hub metadata found"):
            parse_meta(raw_line, "max_link_capacity=2", HubMetadata)

    def test_valid_hub_meta(self) -> None:
        raw_line = RawLine(line=1, key="hub",
                           value="name 0 0 [color=green, zone=normal," \
                                 "max_drones=2]")
        meta = parse_meta(raw_line, "color=green zone=normal max_drones=2",
                          HubMetadata)
        assert meta == HubMetadata(color="green",
                                   zone=Zone.NORMAL,
                                   max_drones=2)

    def test_invalid_connection_meta(self) -> None:
        raw_line = RawLine(line=1, key="connection",
                           value="name1-name2 [color=green]")
        with pytest.raises(MapError, match="Invalid connection metadata found"):
            parse_meta(raw_line, "color=green", ConnectionMetadata)

    def test_valid_connection_meta(self) -> None:
        raw_line = RawLine(line=1, key="connection",
                           value="name1-name2 [max_link_capacity=2]")
        meta = parse_meta(raw_line, "max_link_capacity=2",
                          ConnectionMetadata)
        assert meta == ConnectionMetadata(max_link_capacity=2)