# Local modules
from .pixel_class import Flag
from .dir_class import Dir


class Cell():
    """ This class represent a cell of a maze.

    Attributes:
        _walls (int): Walls around the cell represented by a value (0-15).
        _pos (tuple[int, int]): Position of the cell in the maze grid.
        _locked (bool): Status of the cell. If locked cant be modified.
        _flag (FLAG): Marks the cell with a specific flag, this helps with the
                      drawing of the cell.
    """
    def __init__(self, pos: tuple[int, int], value: int = 15):
        """Initializes the cell with given parameters

        Args:
            pos tuple[int, int]: (x, y) position of the cell in the 2D array
                                 of the maze.
             value (Optional[int]): An optional value of what the wall
                                    configuration is (0 - 15).

        Notes:
            15 represents the 4 bits we need to describe the walls
        """
        Cell._validate(value)
        self._walls = value
        self._pos = pos
        self._locked = False
        self._flag = Flag.EMPTY

    @property
    def locked(self) -> bool:
        """Retuns the locked state of the cell."""
        return self._locked

    @locked.setter
    def locked(self, lock: bool) -> None:
        """Set the lock state of this cell.

        Args:
            lock (bool): If True, this cells walls can no longer be modified.
        """
        self._locked = lock

    @property
    def visited(self) -> bool:
        return self._visited

    @visited.setter
    def visited(self, visit: bool) -> None:
        self._visited = visit

    @property
    def walls(self) -> int:
        """Walls configuration of the cell"""
        return self._walls

    @walls.setter
    def walls(self, value: int) -> None:
        """Set the value of the walls of the cell. This sets ALL walls.

        Args:
            value (int): 0-15 int to represent the wall configuration.
        """
        Cell._validate(value)
        if self._locked:
            raise RuntimeError("Cell is locked")
        self._walls = value

    @property
    def pos(self) -> tuple[int, int]:
        """Position of the cell in the maze as given by (x,y) coordinates."""
        return self._pos

    @property
    def flag(self) -> Flag:
        """
        Return the flag set for this cell
        """
        return self._flag

    @flag.setter
    def flag(self, flag: Flag) -> None:
        """
        Set the flag of this cell to something
        """
        self._flag = flag

    def toggle_wall(self, direction: Dir) -> None:
        """Given a direction it opens or closes the associated wall.

        Args:
            direction (Dir) Direction of the wall from the center of the cell.
        """
        if self._locked:
            raise RuntimeError("Cell is locked")
        if self._walls:
            self._walls = self._walls ^ direction.value

    def get_wall(self, direction: Dir) -> bool:
        """Checks if theres a wall in a given direction.

        Args:
            direction (Dir) Direction of the wall from the center of the cell.

        Returns:
            True or False based on the state of the wall in the direction given
        """
        if self._walls:
            return bool(self._walls & direction.value)
        return False

    @staticmethod
    def _validate(value: int) -> None:
        """Validate walls of a cell. Allowed values are 0-15 ints.

        Args:
            value (int): 0-15 int to represent the wall configuration.

        Raises:
            If value is of incorrect value.
        """
        if (value < 0) or (value > 15):
            raise RuntimeError("Cells cannot have values"
                               " outside the range 0-15."
                               f" Provided value is {value}")
