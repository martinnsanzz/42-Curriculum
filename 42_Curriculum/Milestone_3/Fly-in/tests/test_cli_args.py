# Built-in modules
from argparse import ArgumentError

# Installed modules
import pytest

# Local modules
from src.main.parser import parse_args


def test_wrong_flag(monkeypatch: pytest.MonkeyPatch) -> None:
    monkeypatch.setattr("sys.argv", ["src", "--unknown_flag", "random"])
    with pytest.raises(ArgumentError, match="Incorrect argument on "
                       "command line"):
        parse_args()


def test_with_cli(monkeypatch: pytest.MonkeyPatch) -> None:
    monkeypatch.setattr("sys.argv", ["src", "--visual"])
    args = parse_args()
    assert args.visual is True


def test_without_cli(monkeypatch: pytest.MonkeyPatch) -> None:
    monkeypatch.setattr("sys.argv", ["src"])
    args = parse_args()
    assert args.visual is False
