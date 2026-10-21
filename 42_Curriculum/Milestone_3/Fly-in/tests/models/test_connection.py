# Local modules
from src.main.models import Connection, ConnectionMetadata


def test_valid_connection_with_default_metadata() -> None:
    conn = Connection(line=1, name1="a", name2="b")
    assert conn.line == 1
    assert conn.name1 == "a"
    assert conn.name2 == "b"
    assert conn.metadata is not None
    assert conn.metadata.max_link_capacity == 1


def test_valid_connection_with_metadata() -> None:
    meta = ConnectionMetadata(max_link_capacity=2)
    conn = Connection(line=1, name1="a", name2="b", metadata=meta)
    assert conn.line == 1
    assert conn.name1 == "a"
    assert conn.name2 == "b"
    assert conn.metadata is not None
    assert conn.metadata.max_link_capacity == 2
