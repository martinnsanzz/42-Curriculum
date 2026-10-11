# Installed modules
import pytest
from pydantic import ValidationError

# Local modules
from main.models import Connection, ConnectionMetadata

def test_connection_metadata_is_required() -> None:
    with pytest.raises(ValidationError):
        Connection(line=1, name1="a", name2="b")

def test_valid_connection_without_metadata() -> None:
    conn = Connection(line=1, name1="a", name2="b", metadata=None)
    assert conn.metadata is None

def test_valid_connection_with_metadata() -> None:
    meta = ConnectionMetadata(max_link_capacity=2)
    conn = Connection(line=1, name1="a", name2="b", metadata=meta)
    assert conn.line == 1
    assert conn.name1 == "a"
    assert conn.name2 == "b"
    assert conn.metadata is not None
    assert conn.metadata.max_link_capacity == 2