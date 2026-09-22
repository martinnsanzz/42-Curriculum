#!/usr/bin/env python3


from typing import cast


class C:
    H = '\033[95m'  # Header
    B = '\033[94m'  # Blue
    C = '\033[96m'  # Cyan
    G = '\033[92m'  # Green
    W = '\033[93m'  # Warning
    F = '\033[91m'  # Fail
    E = '\033[0m'  # End
    Bo = '\033[1m'  # Bold
    U = '\033[4m'

    def msg(self, color: str, msg: str) -> None:
        print(getattr(self, color) + msg + C.E)


def artifact_sorter(artifacts: list[dict[str, str | int]]) -> \
        list[dict[str, str | int]]:
    return sorted(artifacts, key=lambda x: x["power"], reverse=True)


def power_filter(mages: list[dict[str, str | int]], min_power: int) -> \
        list[dict[str, str | int]]:
    return list(filter(lambda x: cast(int, x["power"]) >= min_power, mages))


def spell_transformer(spells: list[str]) -> list[str]:
    return list(map(lambda x: '* ' + x + ' *', spells))


def mage_stats(mages: list[dict[str, str | int]]) -> \
        dict[str, dict[str, str | int] | float]:
    max_power = min(mages, key=lambda x: x["power"])
    min_power = max(mages, key=lambda x: x["power"])
    avg_power = sum(map(lambda x: int(x["power"]), mages)) / len(mages)
    return {"max_power": max_power,
            "min_power": min_power,
            "avg_power": avg_power
            }


def display(artifacts: list[dict[str, str | int]],
            mages: list[dict[str, str | int]],
            spells: list[str]) -> None:
    C().msg("Bo", "Sorting artifacts by power level in descending order:")
    print(f"Original -> {artifacts}", end="\n\n")
    print(f"Sorted -> {artifact_sorter(artifacts)}")

    print("\n" + ("=" * 50))
    C().msg("Bo", "Sorting mages by power level:")
    print(f"Original -> {mages}", end="\n\n")
    print(f"Filtered -> {power_filter(mages, 80)}")

    print("\n" + ("=" * 50))
    C().msg("Bo", "Transforming spells:")
    print(f"Original -> {spells}", end="\n\n")
    print(f"Filtered -> {spell_transformer(spells)}")

    print("\n" + ("=" * 50))
    C().msg("Bo", "Showing mages stats:")
    print(f"Original -> {mages}", end="\n\n")
    print(f"Filtered -> {mage_stats(mages)}")


if __name__ == "__main__":
    artifacts: list[dict[str, str | int]] = [
        {"name": "Fire Staff", "power": 40, "type": "Fire"},
        {"name": "Crystal Orb", "power": 2, "type": "Water"},
        {"name": "Nature Scepter", "power": 78, "type": "Grass"},
        {"name": "Stone Hammer", "power": 100, "type": "Rock"}
    ]

    mages: list[dict[str, str | int]] = [
        {"name": "Aldric", "power": 95, "element": "Fire"},
        {"name": "Maris", "power": 65, "element": "Water"},
        {"name": "Theron", "power": 72, "element": "Grass"},
        {"name": "Kael", "power": 80, "element": "Rock"}
    ]

    spells: list[str] = [
        "Aqua Blast",
        "Inferno Strike",
        "Boulder Crush",
        "Holy Light"
    ]

    C().msg("H", "Displaying functions in action:")
    print("=" * 50)
    display(artifacts, mages, spells)
