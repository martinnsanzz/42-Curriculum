# Installed modules
import pytest

# Local modules
from src.main.backend import load_map
from src.main.exceptions import MapError

def test_missing_file(tmp_path):
    with pytest.raises(MapError, match="No such file"):
        load_map(tmp_path / "nope.txt")