#!/usr/bin/env python3

class C:
    H = '\033[95m'  # Header
    B = '\033[94m'  # Blue
    C = '\033[96m'  # Cyan
    G = '\033[92m'  # Green
    W = '\033[93m'  # Warning
    F = '\033[91m'  # Fail
    E = '\033[0m'  # End
    Bo = '\033[1m'  # Bold

    def msg(self, color: str, msg: str):
        print(getattr(self, color) + msg + C.E)


def secure_archieve(file: str, action: int = 0,
                    content: str = "") -> tuple:
    success = False
    if action == 0:
        try:
            with open(file, 'r') as f:
                content = f.read()
                success = True
        except Exception as error:
            content = str(error)
    elif action == 1:
        try:
            with open(file, 'w') as f:
                f.write(content)
                success = True
                content = "Content successfully written to file"
        except Exception as error:
            content = str(error)
    return tuple((success, f"{content}"))


if __name__ == "__main__":
    C().msg("H", "=== Cyber archives Security ===")

    C().msg("C", "\nUsing 'secure_archive to read from a nonexistent file:")
    C().msg("F", str(secure_archieve("Non_existent.txt")))

    C().msg("C", "\nUsing 'secure_archive to read from an  inaccessible file:")
    C().msg("F", str(secure_archieve("no_permission.txt")))

    C().msg("C", "\nUsing 'secure_archive to read from a regular file:")
    C().msg("G", str(secure_archieve("ancient_fragment.txt")))

    C().msg("C", "\nUsing 'secure_archive to write content to a new file")
    with open("ancient_fragment.txt", 'r') as f:
        f_content = f.read()
        C().msg("G", str(secure_archieve("new_file.txt", 1, f_content)))
