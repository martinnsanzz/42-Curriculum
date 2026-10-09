# Built-in modules
from pathlib import Path
from typing import Optional
from re import compile, fullmatch

# Installed modules
from pydantic import PositiveInt, TypeAdapter, ValidationError, BaseModel

# Local modules
from fly_in.models import Connection, Hub, HubMetadata, DroneMap, \
                          ConnectionMetadata, Zone
from fly_in.exceptions import MapError
from .loader import load_map, RawLine

_DRONES = TypeAdapter(PositiveInt)

HUB_PATTERN = compile(
    r'(?P<name>[!-,.-~]+) (?P<x>-?[0-9]+) (?P<y>-?[0-9]+)'
    r'(?: \[(?P<meta>[^\]]*)\])?'
)
CONNECTION_PATTERN = compile(
    r'(?P<name1>[!-,.-~]+)-(?P<name2>[!-,.-~]+)'
    r'(?: (?P<meta>\[[^\]]*\]))?'
)
KV = r'[^\s=]+=[^\s=]+'
METADATA_PATTERN = compile(rf'{KV}(?: {KV}){{0,2}}')

VALID_HUB_METADATA = {"color", "zone", "max_drones"}
VALID_CONNECTION_METADATA = {"max_link_capacity"}

def parse_map(map_path: Path) -> DroneMap:
    start: Optional[Hub] = None
    end: Optional[Hub] = None
    hubs: list[Hub] = []
    connections: list[Connection] = []

    first, *rest = load_map(map_path)

    if first.key != "nb_drones":
        raise MapError("Number of drones must be on top of file. Write "
                "this setting as the first line !!\n" \
                "    Format -> nb_drone: 5\n")

    nb_drones = parse_nb_drones(first)

    for raw in rest:
        match raw.key:
            case "nb_drones":
                raise MapError(f"line {raw.line}: duplicate 'nb_drones'\n")
            case "start_hub":
                if start is not None:
                    raise MapError(f"line {raw.line}: duplicate 'start_hub'\n")
                start = parse_hub(raw)
            case "end_hub":
                if end is not None:
                    raise MapError(f"line {raw.line}: duplicate 'end_hub'\n")
                end = parse_hub(raw)
            case "hub":
                hubs.append(parse_hub(raw))
            case "connection":
                connections.append(parse_connection(raw))

    if start is None or end is None:
        raise MapError("Map needs one 'start_hub' and one 'end_hub'\n")

    try:
        drone_map = DroneMap(
            nb_drones=nb_drones,
            start=start,
            end=end,
            hubs=hubs,
            connections=connections
        )
    except ValidationError as e:
        err = e.errors()[0]
        field = ".".join(str(p) for p in err["loc"])
        raise MapError(f"Unexpected error happened parsing the map.\n" \
                       f"   Invalid '{field}'\n" \
                       f"{err['msg']}\n") from e
    return drone_map

def parse_nb_drones(raw: RawLine) -> int:
    try:
        return _DRONES.validate_python(raw.value)
    except ValidationError as e:
        raise MapError(
            f"Line {raw.line}: nb_drones {e.errors()[0]['msg']}\n"
        ) from e

def parse_hub(raw: RawLine) -> Hub:
    m = HUB_PATTERN.fullmatch(raw.value)

    if m is None:
        raise MapError(f"Incorrect hub format at line {raw.line}.\n" \
                       "    Correct format: name x y [metadata]\n" \
                       "Note: [metadata] is optional\n")
    try:
        return Hub(
            line=raw.line,
            name=m["name"],
            x=m["x"],
            y=m["y"],
            metadata=parse_meta(raw, m["meta"], HubMetadata),
        )
    except ValidationError as e:
        err = e.errors()[0]
        print(err)
        field = ".".join(str(p) for p in err["loc"])
        raise MapError(f"Line {raw.line}: invalid '{field}': "
                       f"{err['msg']}\n") from e

def parse_connection(raw: RawLine) -> Connection:
    m = CONNECTION_PATTERN.fullmatch(raw.value)

    if m is None:
        raise MapError(f"Incorrect connection format at line {raw.line}.\n" \
                       "    Correct format: name1-name2 [metadata]\n" \
                       "Note: [metadata] is optional\n")
    try:
        return Connection(
            line=raw.line,
            name1=m["name1"],
            name2=m["name2"],
            metadata=parse_meta(raw, m["meta"], ConnectionMetadata)
        )
    except ValidationError as e:
        err = e.errors()[0]
        field = ".".join(str(p) for p in err["loc"])
        raise MapError(f"Line {raw.line}: invalid '{field}': "
                       f"{err['msg']}\n") from e

def parse_meta[M: BaseModel](raw: RawLine, meta: Optional[str],
                             model: M) -> Optional[M]:
    if not meta:
        return None
    if meta == "[]":
        raise MapError(f"Found metadata field empty at line {raw.line}\n")

    m = METADATA_PATTERN.fullmatch(meta)
    if meta and m is None:
        raise MapError(f"Incorrect metadata format at line {raw.line}.\n" \
                        "    metadata must be 1 to 3 key=value " \
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
        err = e.errors()[0]
        field = ".".join(str(p) for p in err["loc"])
        raise MapError(f"Found invalid metadata at line '{raw.line}'.\n" \
                       f"   Invalid '{field}'.\n"\
                       f"{err['msg']}\n") from e
