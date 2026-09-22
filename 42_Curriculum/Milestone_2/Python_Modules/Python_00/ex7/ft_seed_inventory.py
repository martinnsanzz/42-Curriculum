def ft_seed_inventory(seed_type: str, quantity: int, unit: str) -> None:
    if (unit == "packets"):
        msg = f"{quantity} packets available"
    elif (unit == "grams"):
        msg = f"{quantity} grams total"
    elif (unit == "area"):
        msg = f"covers {quantity} meters square"
    else:
        print("Unknown unit type")
        return None
    print(f"{seed_type.capitalize()} seeds: {msg}")
