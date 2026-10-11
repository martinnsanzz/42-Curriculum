# Installed modules
import pytest
from pathlib import Path

# Local modules
from main.parser.map_parser import load_map, RawLine
from main.exceptions import MapError

def test_missing_file(tmp_path: Path) -> None:
    with pytest.raises(MapError, match="No such file"):
        load_map(tmp_path / "nope.txt")

def test_directory(tmp_path: Path):
    with pytest.raises(MapError, match="Is a directory"):
        load_map(tmp_path)

def test_no_permissions(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_text("data")
    f.chmod(0o000)
    with pytest.raises(MapError, match="Permission denied"):
        load_map(f)

def test_not_valid_text(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_bytes(b"\xff\xfe\x00\x80")
    with pytest.raises(MapError, match="Not a valid text file"):
        load_map(f)

def test_empty_file(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_text("")
    with pytest.raises(MapError, match="File is empty"):
        load_map(f)

def test_only_spaces(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_text("   \n\n\t\t   ")
    with pytest.raises(MapError, match="File is empty"):
        load_map(f)

def test_only_comments(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_text("# Easy Level 1: Simple linear path\n" \
                 "# Easy Level 2: Simple fork with two paths\n")
    with pytest.raises(MapError, match="File is empty"):
        load_map(f)

def test_no_argument(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_text("start_hub: ")
    with pytest.raises(MapError, match="Key must be followed by arguments"):
        load_map(f)

def test_wrong_line_format(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_text("start_hub start 0 0 [color=green]")
    with pytest.raises(MapError, match="Incorrect line format:"):
        load_map(f)

def test_invalid_key(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_text("unknown_key: start 0 0 [color=green]")
    with pytest.raises(MapError, match="Incorrect key found"):
        load_map(f)

def test_valid_map(tmp_path: Path) -> None:
    f = tmp_path / "map.txt"
    f.write_text("start_hub: start 0 0 [color=green]")
    result = load_map(f)
    assert result == [
        RawLine(line=1, key="start_hub", value="start 0 0 [color=green]")
    ]
