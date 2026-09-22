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

    def print_msg(self, color: str, msg: str):
        print(getattr(self, color) + msg + Colors.ENDC)


def read_file(file: str) -> None:
    f = open(file, "r")
    print("---\n")
    print(f.read())
    f.close()
    print("\n---")
    Colors().print_msg("OKCYAN", f"File '{file}' closed.")


def main(file: str) -> None:
    Colors().print_msg("HEADER", "=== Cyber Archives Recovery ===")
    Colors().print_msg("OKCYAN", f"Accesing file '{file}'")
    try:
        read_file(file)
    except Exception as error:
        Colors().print_msg("FAIL", f"Error opening file '{file}': {error}")


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        Colors().print_msg("WARNING", "Usage: ft_ancient_text.py <file>")
