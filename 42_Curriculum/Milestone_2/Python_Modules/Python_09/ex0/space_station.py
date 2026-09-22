#!/usr/bin/env python3


from pydantic import BaseModel, Field, ValidationError
from datetime import datetime


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


class SpaceStation(BaseModel):
    station_id: str = Field(min_length=3, max_length=10)
    name: str = Field(min_length=1, max_length=50)
    crew_size: int = Field(ge=1, le=20)  # ge = Greater than; le = Less than
    power_level: float = Field(ge=0.0, le=100.0)
    oxygen_level: float = Field(ge=0.0, le=100.0)
    last_maintenance: datetime  # "%Y-%m-%d %H:%M:%S"
    is_operational: bool = Field(default=True)
    notes: str = Field(default="")


if __name__ == "__main__":
    C().msg("H", "Space Station Data Validation")
    print("=" * 30)

    C().msg("Bo", "Valid Station created:")
    valid_station: SpaceStation = SpaceStation(
        station_id="LGW125",
        name="Titan Mining Outpost",
        crew_size=6,
        power_level=76.4,
        oxygen_level=94.5,
        last_maintenance=datetime(2023, 7, 11))  # "2023-07-11T00:00:00"

    print(f"ID: {valid_station.station_id}")
    print(f"Name: {valid_station.name}")
    print(f"Crew: {valid_station.crew_size} people")
    print(f"Power: {valid_station.power_level}%")
    print(f"Oxygen: {valid_station.oxygen_level}%")
    print("Status: ", end="")
    print("Operational") if valid_station.is_operational \
        else print("Non operational")
    print(f"Last Maintenance: {valid_station.last_maintenance}")
    print(f"Notes: {valid_station.notes}") if valid_station.notes \
        else None

    print("=" * 30)
    C().msg("Bo", "Expected Validation Error:")

    try:
        invalid_station: SpaceStation = SpaceStation(
            station_id="LGW125",
            name="Titan Mining Outpost",
            crew_size=21,
            power_level=76.4,
            oxygen_level=94.5,
            last_maintenance=datetime(2023, 7, 11),  # "2023-07-11T00:00:00"
            is_operational=False)
    except ValidationError as e:
        C().msg("F", f" {str(e.errors()[0]['msg'])}")
