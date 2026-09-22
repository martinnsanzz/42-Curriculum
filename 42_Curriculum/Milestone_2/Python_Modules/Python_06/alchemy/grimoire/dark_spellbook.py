from .dark_validator import validate_ingredients


def dark_spell_allowed_ingredients() -> list:
    return ["bats", "frogs", "arsenic", "eyeball"]


def dark_spell_record(spell_name: str, ingredients: str) -> str:

    is_valid: str = validate_ingredients(ingredients)

    if "INVALID" in is_valid:
        return f"Spell rejected: {spell_name}: ({is_valid})"
    else:
        return f"Spell recorded: {spell_name}: ({is_valid})"
