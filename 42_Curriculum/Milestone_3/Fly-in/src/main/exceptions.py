class FlyInError(Exception):
    """Base class for every expected error in the project."""

class MapSelectionError(FlyInError):
    """The maps are unreachable or not present on the library"""

class MapError(FlyInError):
    """The map file is unreadable or its content is invalid."""

class GraphError(FlyInError):
    """The graph or algorithm isnt working accordingly"""

class SimulationError(FlyInError):
    """The graph or algorithm isnt working accordingly"""