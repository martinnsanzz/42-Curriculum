# Installed modules
import pytest
from pathlib import Path

# Local modules
from fly-in.parser.map_parser import load_map
from src.fly-in.exceptions import MapError

def test_missing_file(tmp_path: Path):
    with pytest.raises(MapError, match="No such file"):
        load_map(tmp_path / "nope.txt")

def test_directory(tmp_path: Path):
    with pytest.raises(MapError, match="Is a directory"):
        load_map(tmp_path)

def test_no_permissions(tmp_path: Path):
    f = tmp_path / "map.txt"
    f.write_text("data")
    f.chmod(0o000)
    with pytest.raises(MapError, match="Permission denied"):
        load_map(f)

def test_not_valid_text(tmp_path: Path):
    f = tmp_path / "map.txt"
    f.write_bytes(b"\xff\xfe\x00\x80")
    with pytest.raises(MapError, match="Not a valid text file"):
        load_map(f)

def test_empty_file(tmp_path: Path):
    f = tmp_path / "map.txt"
    f.write_text("")
    with pytest.raises(MapError, match="File is empty"):
        load_map(f)

def test_only_spaces(tmp_path: Path):
    f = tmp_path / "map.txt"
    f.write_text("   \n\n\t\t   ")
    with pytest.raises(MapError, match="File is empty"):
        load_map(f)

def test_wrong_line_format(tmp_path: Path):
    f = tmp_path / "map.txt"
    f.write_text("start_hub start 0 0 [color=green]")
    with pytest.raises(MapError, match="Incorrect line format:"):
        load_map(f)

def test_valid_map(tmp_path: Path):
    f = tmp_path / "map.txt"
    f.write_text("start_hub: start 0 0 [color=green]")
    result = load_map(f)
    assert result == [["start_hub", "start 0 0 [color=green]"]]
