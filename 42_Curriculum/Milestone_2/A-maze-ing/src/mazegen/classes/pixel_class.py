# Build-in modules
from enum import Enum, auto


class Flag(Enum):
    """
    Enum that represents what a pixel should be drawn as.
    The values are:
    EMPTY: An empy cell that is not part of a solution or pattern
    SOLUTION: A cell or a wall that is part of a solution path
    PATTERN: A cell or a wall that is part of a pattern
    WALL: A wall that is not part of a solution or pattern
    """
    WALL = auto()
    EMPTY = auto()
    SOLUTION = auto()
    PATTERN = auto()
    FOCUS = auto()
    EXIT = auto()
    ENTRY = auto()


class Pixel():
    """
    Class to represent a pixel.
    This is different from the Cell class in that a cell
    represents only a path, whereas the pixel includes
    any cells or walls.
    It simply stores Influence of cells which are then resolved
    using the read() method.
    """
    def __init__(self) -> None:
        """Initialises an emtpty pixel with no flags"""
        self._flags: set[Flag] = set()

    def add_flag(self, flag: Flag) -> None:
        """Adds a flag to the pixels list of flags"""
        self._flags.add(flag)

    def read(self) -> Flag:
        """
        Resolve the flags on the pixel.
        This will make sure that a pixel that is influenced by
        two special cells inherits their status
        (eg. the opened wall between two solution cells becomes a solution)

        Returns:
            The resolved flag of the pixel.
        """
        if len(self._flags) == 0:
            return Flag.WALL
        elif len(self._flags) == 1:
            for flag in self._flags:
                return flag
        elif Flag.EMPTY in self._flags:
            return Flag.EMPTY
        elif Flag.SOLUTION in self._flags:
            return Flag.SOLUTION
        elif Flag.FOCUS in self._flags:
            return Flag.FOCUS
        return Flag.WALL
