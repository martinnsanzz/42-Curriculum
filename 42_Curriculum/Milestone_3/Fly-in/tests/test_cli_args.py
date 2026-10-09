# Built-in modules
from pathlib import Path
from argparse import ArgumentError

# Installed modules
import pytest

# Local modules
from src.fly-in.cli import parse_args

def test_wrong_flag(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setattr("sys.argv", ["src", "--unknown_flag", "random"])
    with pytest.raises(ArgumentError, match="The following arguments are required"):
        parse_args()

def test_flag_with_no_arg(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setattr("sys.argv", ["src", "--map"])
    with pytest.raises(ArgumentError, match="expected one argument"):
        parse_args()

def test_right_cli(monkeypatch: pytest.MonkeyPatch):
    monkeypatch.setattr("sys.argv", ["src", "--map", "x.txt"])
    args = parse_args()
    assert args.map == Path("x.txt")