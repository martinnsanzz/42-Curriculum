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
    start_hub: Optional[Hub] = None
    end_hub: Optional[Hub] = None
    hubs: list[Hub] = []
    connections: list[Connection] = []

    first, *rest = load_map(map_path)

    if first.key != "nb_drones":
        raise MapError("Number of drones must be on top of file. Write "
                "this setting as the first line !!\n" \
                "    Format -> nb_drone: <number>\n")

    nb_drones = parse_nb_drones(first)

    for raw in rest:
        match raw.key:
            case "nb_drones":
                raise MapError(f"{DUP_ERROR} '{raw.line}'\n   "\
                               "- Duplicate 'nb_drones'\n")
            case "start_hub":
                if start_hub is not None:
                    raise MapError(f"{DUP_ERROR} '{raw.line}'\n   "\
                                   "- Duplicate 'start_hub'\n")
                start_hub = parse_hub(raw)
            case "end_hub":
                if end_hub is not None:
                    raise MapError(f"{DUP_ERROR} '{raw.line}'\n   "\
                                   "- Duplicate 'end_hub'\n")
                end_hub = parse_hub(raw)
            case "hub":
                hubs.append(parse_hub(raw))
            case "connection":
                connections.append(parse_connection(raw))

    if start_hub is None:
        raise MapError("Map is missing an 'start_hub'\n")
    elif end_hub is None:
        raise MapError("Map is missing an 'end_hub'\n")

    if len(connections) < (len(hubs) + 2) - 1:
        raise MapError("Map has less connections than expected.\n" \
                       "    - Min connections should be (Total hubs - 1).\n")

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
    try:
        return _DRONES.validate_python(raw.value)
    except ValidationError as e:
        msg = e.errors()[0]["msg"].removeprefix("Value error, ")
        raise MapError(f"Invalid nb_drones:\n   {msg}\n") from e

def parse_hub(raw: RawLine) -> Hub:
    m = fullmatch(HUB_PATTERN, raw.value)

    if m is None:
        raise MapError(f"Incorrect hub format at line {raw.line}.\n" \
                       "    Correct format: <name> <x> <y> [metadata]\n" \
                       "Note: [metadata] is optional.\n")
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
                       f"   {msg}\n") from e

def parse_connection(raw: RawLine) -> Connection:
    m = fullmatch(CONNECTION_PATTERN, raw.value)

    if m is None:
        raise MapError(f"Incorrect connection format at line {raw.line}.\n" \
                       "    Correct format: <name1>-<name2> [metadata]\n" \
                       "Note: [metadata] is optional\n")
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
                       f"   {msg}\n") from e

def parse_meta[M: BaseModel](raw: RawLine, meta: Optional[str],
                             model: type[M]) -> M:
    if not meta:
        return model()
    if meta == "[]":
        raise MapError(f"Found metadata field empty at line {raw.line}\n")

    meta = meta.strip("[]")
    m = fullmatch(METADATA_PATTERN, meta)
    if meta and m is None:
        raise MapError(f"Incorrect metadata format at line {raw.line}.\n" \
                        "   Metadata must be 1 to 3 key=value " \
                        "pairs separated by 1 space\n")

    pairs = [item.split("=", 1) for item in meta.split()]
    keys = [k for k, _ in pairs]

    if raw.key == "connection":
        extra = set(keys) - VALID_CONNECTION_METADATA
        if extra:
            raise MapError("Invalid connection metadata found at " \
                           f"line '{raw.line}'\n" \
                           f"   - Found -> {extra}\n" \
                           f"Allowed {VALID_CONNECTION_METADATA}\n")
    if raw.key.endswith("hub"):
        extra = set(keys) - VALID_HUB_METADATA
        if extra:
            raise MapError("Invalid hub metadata found at " \
                           f"line '{raw.line}'\n" \
                           f"   - Found -> {extra}\n" \
                           f"Allowed {VALID_HUB_METADATA}\n")

    if len(keys) != len(set(keys)):
        raise MapError(f"Found repeated metadata key at line {raw.line}\n")

    try:
        return model.model_validate(dict(pairs), extra="forbid")
    except ValidationError as e:
        msg = e.errors()[0]["msg"].removeprefix("Value error, ")
        raise MapError(f"Invalid metadata at line {raw.line}:\n" \
                       f"   {msg}\n") from e
