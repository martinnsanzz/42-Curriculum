#!/usr/bin/env python3

import sys

if __name__ == "__main__":
    count: int = 1

    print("=== Command Quest ===")
    print(f"Program name: {sys.argv[0]}")

    if len(sys.argv) == 1:
        print("No arguemnts provided!")
    else:
        print(f"Arguments received: {len(sys.argv) - 1}")
        total_args = len(sys.argv)
        for arg in sys.argv[1: len(sys.argv)]:
            print(f"Argument {count}: {arg}")
            count += 1
    print(f"Total arguments: {len(sys.argv)}")
