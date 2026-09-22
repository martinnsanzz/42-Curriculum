#!/usr/bin/env python3


import sys


class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

    def msg(self, color: str, msg: str, std: str = "out"):
        if std == "err":
            sys.stderr.write(getattr(self, color) + msg + Colors.ENDC)
        elif std == "out":
            sys.stdout.write(getattr(self, color) + msg + Colors.ENDC + '\n')


class CustomError(Exception):
    def __init__(self, msg: str) -> None:
        super().__init__(msg)


def read_file(file: str) -> None:
    f = open(file, "r")
    print("---\n")
    print(f.read())
    print("\n---")
    f.close()
    Colors().msg("OKCYAN", f"File '{file}' closed.")


def print_edit_file(file: str) -> None:
    Colors().msg("OKCYAN", "\nTransform data:")
    f = open(file, "r")
    text = f.read().split('\n')
    print("---\n")
    for line in text:
        print(line + '#')
    print("\n---")
    f.close()


def get_new_file_name() -> str:
    print(Colors.OKCYAN, end="", flush=True)
    print("Enter new file name (or empty): ", end="", flush=True)
    print(Colors.ENDC, end="", flush=True)
    user_name = sys.stdin.readline().strip('\n')

    if not user_name:
        raise CustomError("No file name provided, not saving data")
    else:
        return user_name


def create_new_file(file: str, user_name: str) -> None:
    try:
        new_file = open(user_name, "w")
    except Exception as error:
        Colors().msg("FAIL", f"[STDERR] Error opening '{user_name}':", "err")
        Colors().msg("FAIL", f" {error}", "err")
        print()
        return
    Colors().msg("OKGREEN", f"Saving data to '{user_name}'")
    Colors().msg("OKGREEN", f"Data saved in file '{user_name}'")
    f = open(file, "r")
    text = f.read().split('\n')
    index = 0
    for line in text:
        if index != (len(text) - 1):
            new_file.write(line + "#" + '\n')
        else:
            new_file.write(line + "#")
        index += 1
    f.close()
    new_file.close()


def main(file: str) -> None:
    Colors().msg("HEADER", "=== Archives Recovery & Preservation ===")
    Colors().msg("OKCYAN", f"Accesing file '{file}'")
    try:
        read_file(file)
        print_edit_file(file)
        user_name = get_new_file_name()
        create_new_file(file, user_name)
    except CustomError as error:
        Colors().msg("FAIL", f"[STDERR] {error}: ", "err")
    except Exception as error:
        Colors().msg("FAIL", f"[STDERR] Error opening '{file}'", "err")
        Colors().msg("FAIL", f"{error}", "err")
        print()


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        Colors().msg("WARNING", "Usage: ft_ancient_text.py <file>")
