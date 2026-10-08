# Built-in modules
from pathlib import Path

# Local modules
from .hub_validation import check_special_hubs, check_dup_hub_names, \
                            check_hub_coordinates, check_hub_metadata
from ..loader import load_map, RawLine
from ...exceptions import MapError

SPECIAL_HUBS = ["start_hub", "end_hub"]

def parse_map(map_path: Path) -> None:
    map_content = load_map(map_path)

    check_drones(map_content)
    check_raw_hubs(map_content[1:])
    check_raw_connections(map_content[1:])


    return None

def check_drones(map_content: list[RawLine]) -> None:
    seem_names: list[str] = []
    if map_content[0].key != "nb_drones":
        raise MapError("Number of drones must be on top of file. Write "
                       "this setting as the first setting !!\n" \
                       "    Format -> nb_drone: 5\n")
    if not len(map_content[0].value.split()) == 1:
        raise MapError("Number of drones settings must follow this format:\n"
                       "    - nb_drone: 5\n")
    if not map_content[0].value.isdigit():
        raise MapError("Number of drones must be an integer !!\n")

    for line in map_content:
        if line.key == "nb_drones" and line.key in seem_names:
            raise MapError(f"Can't have 2 'nb_drones':\n "
                           f"   -'{line.key}' found again "
                           f"at line [{line.line}]\n")
        seem_names.append(line.key)

    num_drones: int =  int(map_content[0].value)

    if num_drones <= 0:
        raise MapError("Number of drones must be greater than 0 !!\n")


def check_raw_hubs(map_content: list[RawLine]) -> None:
    special_hubs: list[RawLine] = []
    hubs: list[RawLine] = []

    for line in map_content:
        if line.key in SPECIAL_HUBS:
            special_hubs.append(line)

    for line in map_content:
        if line.key.endswith("hub"):
            hubs.append(line)

    check_special_hubs(special_hubs)
    check_dup_hub_names(hubs)
    check_hub_coordinates(hubs)
    check_hub_metadata(hubs)

def check_raw_connections(map_content: list[RawLine]) -> None:
    pass