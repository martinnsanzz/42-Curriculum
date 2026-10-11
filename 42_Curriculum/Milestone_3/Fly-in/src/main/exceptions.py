class FlyInError(Exception):
    """Base class for every expected error in the project."""


class MapError(FlyInError):
    """The map file is unreadable or its content is invalid."""