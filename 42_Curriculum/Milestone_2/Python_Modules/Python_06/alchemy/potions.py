from elements import create_fire, create_water  # Absolute import "From root"
from .elements import create_earth, create_air  # Relative import


def healing_potion() -> str:
    return f"Healing potion brewed with \
'{create_earth()}' and '{create_air()}"


def strength_potion() -> str:
    return f"Strength potion brewed with \
'{create_fire()}' and '{create_water()}"


# Absolute: specifies full path from root. from elements import ...
#   finds elements.py anywhere in your path, starting from root.
#
# Relative: specifies path from current package. ((.elements = current dir),
#    (..elements = parent dir). Works only if the parent is a package
#    (has __init__.py).
