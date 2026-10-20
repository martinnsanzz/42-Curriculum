# Built-in modules
from pathlib import Path
from typing import Optional
from re import compile, fullmatch

# Installed modules
from pydantic import PositiveInt, TypeAdapter, ValidationError, BaseModel

# Local modules
from ..models import Connection, Hub, HubMetadata, DroneMap, \
                          ConnectionMetadata
from ..exceptions import MapError
from .loader import load_map, RawLine

_DRONES = TypeAdapter(PositiveInt)

HUB_PATTERN = compile(
    r'(?P<name>[\x21-\x7e]+) (?P<x>-?[0-9]+) (?P<y>-?[0-9]+)'
    r'(?: (?P<meta>\[[^\]]*\]))?'
)
CONNECTION_PATTERN = compile(
    r'(?P<name1>[\x21-\x7e]+)-(?P<name2>[\x21-\x7e]+)'
    r'(?: (?P<meta>\[[^\]]*\]))?'
)
KV = r'[^\s=]+=[^\s=]+'
METADATA_PATTERN = compile(rf'{KV}(?: {KV}){{0,2}}')

VALID_HUB_METADATA = {"color", "zone", "max_drones"}
VALID_CONNECTION_METADATA = {"max_link_capacity"}

DUP_ERROR = "Found duplicate key on line"

def parse_map(map_path: Path) -> DroneMap:
    """Parse a map file into a validated DroneMap.

    Load the file, require `nb_drones` as the first setting, then collect
    the start hub, end hub, intermediate hubs and connections. Run the
    cross-field checks and build the final model.

    Args:
        map_path (Path): Path to the map file to parse.

    Returns:
        DroneMap: The validated map with the drone count, start and end
            hubs, intermediate hubs and connections.

    Raises:
        MapError: If `nb_drones` is not the first setting, or is repeated.
        MapError: If `start_hub` or `end_hub` is duplicated or missing.
        MapError: If a hub, connection or metadata line is malformed (raised
            by `parse_hub`, `parse_connection` and `parse_meta`).
        MapError: If there are fewer connections than the total number of
            hubs (including start and end) minus one.
        MapError: If `DroneMap` validation fails. The original
            `ValidationError` is chained as the cause.
    """
    start_hub: Optional[Hub] = None
    end_hub: Optional[Hub] = None
    hubs: list[Hub] = []
    connections: list[Connection] = []

    first, *rest = load_map(map_path)

    if first.key != "nb_drones":
        raise MapError("Number of drones must be on top of file. Write "
                "this setting as the first line !!\n" \
                "    Format -> nb_drones: <number>")

    nb_drones = parse_nb_drones(first)

    for raw in rest:
        match raw.key:
            case "nb_drones":
                raise MapError(f"{DUP_ERROR} '{raw.line}'\n   "\
                               "- Duplicate 'nb_drones'")
            case "start_hub":
                if start_hub is not None:
                    raise MapError(f"{DUP_ERROR} '{raw.line}'\n   "\
                                   "- Duplicate 'start_hub'")
                start_hub = parse_hub(raw)
            case "end_hub":
                if end_hub is not None:
                    raise MapError(f"{DUP_ERROR} '{raw.line}'\n   "\
                                   "- Duplicate 'end_hub'")
                end_hub = parse_hub(raw)
            case "hub":
                hubs.append(parse_hub(raw))
            case "connection":
                connections.append(parse_connection(raw))

    if start_hub is None:
        raise MapError("Map is missing an 'start_hub'")
    elif end_hub is None:
        raise MapError("Map is missing an 'end_hub'")

    total_hubs = len(hubs) + 2
    min_connections = total_hubs - 1
    if len(connections) < min_connections:
        raise MapError("Map has less connections than expected.\n" \
                       "    - Min connections should be (Total hubs - 1).")

    try:
        drone_map = DroneMap(
            nb_drones=nb_drones,
            start=start_hub,
            end=end_hub,
            hubs=hubs,
            connections=connections
        )
    except ValidationError as e:
        msg = e.errors()[0]["msg"].removeprefix("Value error, ")
        raise MapError(f"Invalid map:\n {msg}") from e
    return drone_map

def parse_nb_drones(raw: RawLine) -> int:
    """Validate the `nb_drones` setting and return it as an integer.

    Args:
        raw (RawLine): The raw line holding the `nb_drones` setting. Its
            `value` attribute is validated by `_DRONES`.

    Returns:
        int: The validated number of drones.

    Raises:
        MapError: If validation fails. The original `ValidationError` is
            chained as the cause.
    """
    try:
        return _DRONES.validate_python(raw.value)
    except ValidationError as e:
        msg = e.errors()[0]["msg"].removeprefix("Value error, ")
        raise MapError(f"Invalid nb_drones:\n   {msg}") from e

def parse_hub(raw: RawLine) -> Hub:
    """Parse a hub line into a Hub model.

    Match the line value against `HUB_PATTERN` and extract the name,
    integer coordinates and optional metadata.

    Args:
        raw (RawLine): The raw hub line (`start_hub`, `end_hub` or `hub`)
            with the format `<name> <x> <y> [metadata]`.

    Returns:
        Hub: The hub built from the line, including its source line number
            and parsed metadata.

    Raises:
        MapError: If the line does not match `HUB_PATTERN`.
        MapError: If the metadata is invalid (raised by `parse_meta`).
        MapError: If `Hub` validation fails. The original `ValidationError`
            is chained as the cause.
    """
    m = fullmatch(HUB_PATTERN, raw.value)

    if m is None:
        raise MapError(f"Incorrect hub format at line {raw.line}.\n" \
                       "    Correct format: <name> <x> <y> [metadata]\n" \
                       "Note: [metadata] is optional.")
    try:
        return Hub(
            line=raw.line,
            name=m["name"],
            x=int(m["x"]),
            y=int(m["y"]),
            metadata=parse_meta(raw, m["meta"], HubMetadata),
        )
    except ValidationError as e:
        msg = e.errors()[0]["msg"].removeprefix("Value error, ")
        raise MapError(f"Invalid hub at line {raw.line}:\n" \
                       f"   {msg}") from e

def parse_connection(raw: RawLine) -> Connection:
    """Parse a connection line into a Connection model.

    Match the line value against `CONNECTION_PATTERN` and extract the two
    hub names and optional metadata.

    Args:
        raw (RawLine): The raw connection line with the format
            `<name1>-<name2> [metadata]`.

    Returns:
        Connection: The connection built from the line, including its
            source line number and parsed metadata.

    Raises:
        MapError: If the line does not match `CONNECTION_PATTERN`.
        MapError: If the metadata is invalid (raised by `parse_meta`).
        MapError: If `Connection` validation fails. The original
            `ValidationError` is chained as the cause.
    """
    m = fullmatch(CONNECTION_PATTERN, raw.value)

    if m is None:
        raise MapError(f"Incorrect connection format at line {raw.line}.\n" \
                       "    Correct format: <name1>-<name2> [metadata]\n" \
                       "Note: [metadata] is optional")
    try:
        return Connection(
            line=raw.line,
            name1=m["name1"],
            name2=m["name2"],
            metadata=parse_meta(raw, m["meta"], ConnectionMetadata)
        )
    except ValidationError as e:
        msg = e.errors()[0]["msg"].removeprefix("Value error, ")
        raise MapError(f"Invalid connection at line {raw.line}:\n" \
                       f"   {msg}") from e

def parse_meta[M: BaseModel](raw: RawLine, meta: Optional[str],
                             model: type[M]) -> M:
    """Parse a bracketed `key=value` metadata string into a model.

    Strip the surrounding brackets, check the format, reject keys not
    allowed for the line kind, reject repeated keys, then validate the
    pairs against `model` with extra fields forbidden.

    Args:
        raw (RawLine): The line the metadata belongs to. Its `key`
            selects the allowed metadata keys: `"connection"` or any key
            ending in `"hub"`. Its `line` is used in error messages.
        meta (Optional[str]): The raw metadata text including brackets
            (e.g. `"[color=red zone=blocked]"`), or `None`/empty if the
            line has no metadata.
        model (type[M]): The Pydantic model class to validate against.

    Returns:
        M: A `model` instance built from the pairs, or a default `model()`
            if `meta` is `None` or empty.

    Raises:
        MapError: If the metadata is the empty `"[]"`.
        MapError: If the text does not match `METADATA_PATTERN`.
        MapError: If a key is not in `VALID_CONNECTION_METADATA` or
            `VALID_HUB_METADATA` for the line kind.
        MapError: If a key appears more than once.
        MapError: If `model` validation fails. The original
            `ValidationError` is chained as the cause.
    """
    if not meta:
        return model()
    if meta == "[]":
        raise MapError(f"Found metadata field empty at line {raw.line}")

    meta = meta.strip("[]")
    m = fullmatch(METADATA_PATTERN, meta)
    if meta and m is None:
        raise MapError(f"Incorrect metadata format at line {raw.line}.\n" \
                        "   Metadata must be key=value " \
                        "pairs separated by 1 space")

    pairs = [item.split("=", 1) for item in meta.split()]
    keys = [k for k, _ in pairs]

    if raw.key == "connection":
        extra = set(keys) - VALID_CONNECTION_METADATA
        if extra:
            raise MapError("Invalid connection metadata found at " \
                           f"line '{raw.line}'\n" \
                           f"   - Found -> {extra}\n" \
                           f"Allowed {VALID_CONNECTION_METADATA}")
    if raw.key.endswith("hub"):
        extra = set(keys) - VALID_HUB_METADATA
        if extra:
            raise MapError("Invalid hub metadata found at " \
                           f"line '{raw.line}'\n" \
                           f"   - Found -> {extra}\n" \
                           f"Allowed {VALID_HUB_METADATA}")

    if len(keys) != len(set(keys)):
        raise MapError(f"Found repeated metadata key at line {raw.line}")

    try:
        return model.model_validate(dict(pairs), extra="forbid")
    except ValidationError as e:
        msg = e.errors()[0]["msg"].removeprefix("Value error, ")
        raise MapError(f"Invalid metadata at line {raw.line}:\n" \
                       f"   {msg}") from e
