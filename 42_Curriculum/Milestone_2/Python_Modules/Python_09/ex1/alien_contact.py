#!/usr/bin/env python3


from pydantic import BaseModel, Field, model_validator, ValidationError
from datetime import datetime
from enum import Enum


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


class ContactType(Enum):
    RADIO = "radio"
    VISUAL = "visual"
    PHYSICAL = "physical"
    TELEPATHIC = "telepathic"


class AlienContact(BaseModel):
    contact_id: str = Field(min_length=5, max_length=15)
    timestamp: datetime
    location: str = Field(min_length=3, max_length=100)
    contact_type: ContactType
    signal_strength: float = Field(gt=0.0, lt=10.0)
    duration_minutes: int = Field(gt=1, lt=1440)
    witness_count: int = Field(gt=1, lt=100)
    message_received: str = Field(default="", max_length=500)
    is_verified: bool = Field(default=False)

    @model_validator(mode='after')
    def custom_validation_rules(self) -> 'AlienContact':
        if not self.contact_id.startswith("AC"):
            raise ValueError("Contact_ID must start with 'AC'")

        if self.contact_type == ContactType.PHYSICAL \
                and not self.is_verified:
            raise ValueError("Physical contact reports must be verified")

        if self.contact_type == ContactType.TELEPATHIC \
                and self.witness_count < 3:
            raise ValueError("Telepathic contact requires at \
least 3 witnesses")

        if self.signal_strength > 7.0 and not self.message_received:
            raise ValueError("Strong signals (> 7.0) should include \
received messages")
        return self


if __name__ == "__main__":
    C().msg("H", "Alien Contact Log Validation")
    print("=" * 30)

    valid_contact: AlienContact = AlienContact(
        contact_id="AC_2024_001",
        timestamp=datetime(2024, 1, 20),
        location="Atacama Desert, Chile",
        contact_type=ContactType.TELEPATHIC,
        signal_strength=8.0,
        duration_minutes=99,
        witness_count=5,
        message_received="Greetings from Zeta Reticuli",
        is_verified=False
    )

    C().msg("Bo", "Valid contact report: ")
    print(f"ID: {valid_contact.contact_id}")
    print(f"Type: {valid_contact.contact_type.value}")
    print(f"Location: {valid_contact.location}")
    print(f"Signal: {valid_contact.signal_strength}/10")
    print(f"Duration: {valid_contact.duration_minutes} minutes")
    print(f"Witnesses: {valid_contact.witness_count}")
    print(f"Message: {valid_contact.message_received}")

    print("=" * 30)
    C().msg("Bo", "Expected validation error: ")
    try:
        invalid_contact: AlienContact = AlienContact(
            contact_id="AB_2024_001",
            timestamp=datetime(2024, 1, 20),
            location="Atacama Desert, Chile",
            contact_type=ContactType.TELEPATHIC,
            signal_strength=8.0,
            duration_minutes=99,
            witness_count=5,
            message_received="Greetings from Zeta Reticuli",
            is_verified=False
        )
    except ValidationError as e:
        C().msg("F", f" {str(e.errors()[0]['msg'])}")
