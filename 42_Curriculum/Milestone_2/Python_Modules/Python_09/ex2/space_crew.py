#!/usr/bin/env python3


from pydantic import BaseModel, Field, model_validator, ValidationError
from enum import Enum
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


class Rank(Enum):
    CAPTAIN = "captain"
    VICE_CAPTAIN = "vice-captain"
    SNIPER = "sniper"
    COOK = "cook"
    DOCTOR = "doctor"
    NAVIGATOR = "navigator"


class CrewMember(BaseModel):
    member_id: str = Field(min_length=3, max_length=10)
    name: str = Field(min_length=2, max_length=50)
    rank: Rank
    age: int = Field(gt=18, lt=50)
    specialization: str = Field(min_length=3, max_length=30)
    years_experience: int = Field(gt=0, lt=50)
    is_active: bool = Field(default=True)


class CrewMission(BaseModel):
    mission_id: str = Field(min_length=5, max_length=15)
    mission_name: str = Field(min_length=3, max_length=100)
    destination: str = Field(min_length=3, max_length=50)
    launch_date: datetime
    duration_days: int = Field(gt=1, lt=3650)
    crew: list[CrewMember] = Field(min_length=1, max_length=12)
    mission_status: str = Field(default="Planned")
    budget_millions: float = Field(gt=1.0, lt=10000.0)

    @model_validator(mode='after')
    def mission_validation_rules(self) -> 'CrewMission':
        valid_crew: int = 0
        experienced_member: int = 0

        if not self.mission_id.startswith('M'):
            raise ValueError("Mission ID must start with 'M'")

        for member in self.crew:
            if member.rank.value == Rank.CAPTAIN.value or \
               member.rank.value == Rank.VICE_CAPTAIN.value:
                valid_crew += 1
            if not member.is_active:
                raise ValueError("All crew members must be active")
            if member.years_experience > 5:
                experienced_member += 1

        if valid_crew == 0:
            raise ValueError("Crew must have at least one Vice Captain \
or Captain")
        if self.duration_days > 365 and not \
           experienced_member >= float(len(self.crew) / 2):
            raise ValueError(" Long missions (> 365 days) need \
50% experienced crew (5+ years)")

        return self


if __name__ == "__main__":
    C().msg("H", "Straw Hats Mission Crew Validation")
    print("=" * 30)

    luffy: CrewMember = CrewMember(
        member_id="MEM_00147",
        name="Luffy",
        rank=Rank.CAPTAIN,
        age=20,
        specialization="Physical Fighting",
        years_experience=4
    )
    zoro: CrewMember = CrewMember(
        member_id="MEM_00238",
        name="Zoro",
        rank=Rank.VICE_CAPTAIN,
        age=23,
        specialization="Swordmanship",
        years_experience=17
    )
    nami: CrewMember = CrewMember(
        member_id="MEM_00156",
        name="Nami",
        rank=Rank.NAVIGATOR,
        age=20,
        specialization="Navigation",
        years_experience=15
    )
    chooper: CrewMember = CrewMember(
        member_id="MEM_00147",
        name="Chooper",
        rank=Rank.DOCTOR,
        age=19,
        specialization="Taking care of people",
        years_experience=7
    )
    ussop: CrewMember = CrewMember(
        member_id="MEM_00412",
        name="Ussop",
        rank=Rank.SNIPER,
        age=20,
        specialization="Long distance attacks",
        years_experience=4
    )
    valid_crew_list: list[CrewMember] = [luffy, zoro, nami,
                                         chooper, ussop]

    valid_mission: CrewMission = CrewMission(
            mission_id="M_0004",
            mission_name="Bring back Sanji",
            destination="Whole Cake",
            launch_date=datetime(2017, 4, 9),
            duration_days=500,
            crew=valid_crew_list,
            mission_status="Early stages",
            budget_millions=7.0
        )

    C().msg("Bo", "Valid mission Created")
    print(f"Mission: {valid_mission.mission_name}")
    print(f"Type: {valid_mission.mission_id}")
    print(f"Destination: {valid_mission.destination}")
    print(f"Duration: {valid_mission.duration_days} days")
    print(f"Budget: ฿{valid_mission.budget_millions}M berries")
    print(f"Crew Size: {len(valid_mission.crew)}")
    print("Crew members: ")

    for member in valid_mission.crew:
        print(f"- {member.name} ({member.rank.value}) \
- {member.specialization}")

#######################################################
########################################################
########################################################
########################################################

    print()
    print("=" * 30)
    C().msg("Bo", "Expected validation error")

    _luffy: CrewMember = CrewMember(
        member_id="MEM_00147",
        name="Luffy",
        rank=Rank.CAPTAIN,
        age=20,
        specialization="Physical Fighting",
        years_experience=10
    )
    _zoro: CrewMember = CrewMember(
        member_id="MEM_00238",
        name="Zoro",
        rank=Rank.VICE_CAPTAIN,
        age=23,
        specialization="Swordmanship",
        years_experience=10
    )
    _nami: CrewMember = CrewMember(
        member_id="MEM_00156",
        name="Nami",
        rank=Rank.NAVIGATOR,
        age=20,
        specialization="Navigation",
        years_experience=4
    )
    _sanji: CrewMember = CrewMember(
        member_id="MEM_00349",
        name="Sanji",
        rank=Rank.COOK,
        age=22,
        specialization="Cooking",
        years_experience=4,
        is_active=False
    )
    _chooper: CrewMember = CrewMember(
        member_id="MEM_00147",
        name="Chooper",
        rank=Rank.DOCTOR,
        age=19,
        specialization="Taking care of people",
        years_experience=3
    )
    _ussop: CrewMember = CrewMember(
        member_id="MEM_00412",
        name="Ussop",
        rank=Rank.SNIPER,
        age=20,
        specialization="Long distance attacks",
        years_experience=10
    )
    invalid_crew_list: list[CrewMember] = [_luffy, _zoro, _sanji, _nami,
                                           _chooper, _ussop]

    try:
        invalid_mission: CrewMission = CrewMission(
            mission_id="M_0004",
            mission_name="Bring back Sanji",
            destination="Whole Cake",
            launch_date=datetime(2017, 4, 9),
            duration_days=500,
            crew=invalid_crew_list,
            mission_status="Early stages",
            budget_millions=7.0
        )
    except ValidationError as e:
        C().msg("F", f" {str(e.errors()[0]['msg'])}")
