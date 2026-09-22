def light_spell_allowed_ingredients() -> list:
    return ["earth", "air", "fire", "water"]


def light_spell_record(spell_name: str, ingredients: str) -> str:
    from .light_validator import validate_ingredients

    is_valid: str = validate_ingredients(ingredients)

    if "INVALID" in is_valid:
        return f"Spell rejected: {spell_name}: ({is_valid})"
    else:
        return f"Spell recorded: {spell_name}: ({is_valid})"
