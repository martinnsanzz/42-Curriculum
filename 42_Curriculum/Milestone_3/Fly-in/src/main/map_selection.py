# Built-in modules
from typing import Optional
from pathlib import Path
from argparse import Namespace
import curses as c
import os

# Local modules
from .exceptions import MapSelectionError


def resolve_map(args: Namespace) -> Optional[Path]:
    """Return the map file path from the CLI arguments.

    If user quits on the interactive menu it will return None.
    """
    if args.map is not None:
        return Path(args.map)
    return choose_map()


def choose_map() -> Optional[Path]:
    """Prompt the user to select a map through an interactive curses menu.

    Scan the 'maps' folder, then display a two-level menu: first a
    subdirectory, then a map file inside it.

    Returns:
        Path to the selected map file (e.g. ``maps/<folder>/<name>.txt``),
        relative to the current working directory, or None if the user
        quit the menu.

    Raises:
        MapSelectionError: If the 'maps' folder is missing, has no
            subdirectories, or contains a subdirectory without .txt files
            (raised by `read_map_folder`).
    """
    map_struct = read_map_folder()
    return c.wrapper(run_menu, map_struct)


def read_map_folder() -> dict[str, list[str]]:
    """Read the 'maps' folder and index its subdirectories and .txt files.

    The 'maps' folder is resolved relative to the current working
    directory. Only direct subdirectories are scanned, and only files
    ending in '.txt' are kept.

    Returns:
        Dictionary mapping each subdirectory path (prefixed with
        'maps/', e.g. ``"maps/easy"``) to the alphabetically sorted list
        of its .txt file names (file names only, not full paths).

    Raises:
        MapSelectionError: If 'maps' is not a directory, if it contains
            no subdirectories, or if any subdirectory contains no .txt
            files.
    """
    if not Path("maps").is_dir():
        raise MapSelectionError("Couldn't find 'maps' directory in project.")

    subdirs = ["maps/" + dir for dir in os.listdir("maps")
               if os.path.isdir("maps/" + dir)]

    if not subdirs:
        raise MapSelectionError("Folder 'maps' does not contain any "
                                "subdirectories.")

    map_struct: dict[str, list[str]] = {}
    for dir in subdirs:
        maps = [map for map in os.listdir(dir)
                if map.endswith(".txt")]

        if not maps:
            raise MapSelectionError(f"Folder '{dir}' doesn't contain any "
                                    "maps.\n Delete it or add maps to it.")
        map_struct.update({dir: sorted(maps)})
    return map_struct


def run_menu(stdscr: c.window,
             map_struct: dict[str, list[str]]) -> Optional[Path]:
    """Run the interactive two-level map selection loop.

    Designed to be passed to `curses.wrapper`. Level 1 lists the map
    folders; level 2 lists the maps of the chosen folder. The cursor
    wraps around at both ends of a list.

    Keys:
        Up: Move selection up.
        Down: Move selection down.
        Enter: Open the highlighted folder, or select the highlighted map.
        Left: Go back from a map list to the folder list.
        q: Quit without selecting.

    Args:
        stdscr: Main curses window provided by `curses.wrapper`.
        map_struct: Mapping of folder paths to their map file names, as
            returned by `read_map_folder`.

    Returns:
        Path built from the selected folder and map file name, or None
        if the user pressed 'q'.
    """
    c.curs_set(0)
    dirs = list(map_struct)
    current: str | None = None
    idx = 0

    while True:
        entries = dirs if current is None else map_struct[current]

        draw_menu(stdscr, entries, idx, current)
        key = stdscr.getch()

        if key == c.KEY_UP:
            idx = (idx - 1) % len(entries)
        elif key == c.KEY_DOWN:
            idx = (idx + 1) % len(entries)
        elif key in (c.KEY_ENTER, 10, 13):
            if idx < len(entries):
                if current is None:
                    current = entries[idx]
                    idx = 0
                else:
                    return Path(current) / entries[idx]
            else:
                if current is not None:
                    idx = dirs.index(current)
                current = None
        elif key == ord("q"):
            return None
        elif key == c.KEY_LEFT and current is not None:
            idx = dirs.index(current)
            current = None


def draw_menu(stdscr: c.window, items: list[str], idx: int,
              current: Optional[str]) -> None:
    """Render the current menu level on the curses screen."""
    stdscr.erase()
    h, w = stdscr.getmaxyx()
    title = "Select a map folder" if current is None \
            else f"{current.removeprefix("maps/").capitalize()} Levels"
    stdscr.addnstr(0, 0, title, w - 1, c.A_BOLD)

    for i, item in enumerate(items[: h - 3]):
        attr = c.A_REVERSE if i == idx else c.A_NORMAL
        if item.startswith("maps/"):
            clean = item.removeprefix("maps/").capitalize()
            stdscr.addnstr(i + 2, 2, clean, w - 3, attr)
        else:
            stdscr.addnstr(i + 2, 2, item, w - 3, attr)
    stdscr.refresh()
