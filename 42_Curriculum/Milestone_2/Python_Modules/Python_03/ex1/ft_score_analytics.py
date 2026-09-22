#!/usr/bin/env python3


import sys


def process_lst(argv: list) -> list:
    lst: list = []

    for arg in argv[1:len(argv)]:
        try:
            lst.append(int(arg))
        except Exception:
            print(f"Invalid parameter: '{arg}'")
    return lst


if __name__ == "__main__":
    print("=== Player Score Analytics ===")

    lst: list = process_lst(sys.argv)
    if lst:
        print(f"Scores processed: {lst}")
        print(f"Total players: {len(lst)}")
        print(f"Total score: {sum(lst)}")
        print(f"Avarage score: {sum(lst) / len(lst)}")
        print(f"High score: {max(lst)}")
        print(f"Low score: {min(lst)}")
        print(f"Score range: {max(lst) - min(lst)}")
    else:
        print("No scores provided. Usage: python3 ", end="")
        print("ft_score_analytics.py <score1> <score2> ...")
