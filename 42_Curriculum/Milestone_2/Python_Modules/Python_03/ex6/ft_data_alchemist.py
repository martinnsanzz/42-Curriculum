#!/usr/bin/env python3


import random


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


class Players():
    def __init__(self) -> None:
        self.player_list: list = [
            "Alice",
            "bob",
            "Charlie",
            "dylan",
            "Emma",
            "Gregory",
            "john",
            "kevin",
            "Liam"
        ]


if __name__ == "__main__":
    print(Colors.HEADER + "=== Game Data Alchemist ===" + Colors.ENDC)

    print(Colors.OKCYAN + "Initial list of players: ", end="" + Colors.ENDC)
    init_lst: list = Players().player_list
    print(init_lst)

    print(Colors.OKCYAN + "\nNew list with all names capitalized: ", end="")
    print(Colors.ENDC, end="")
    cap_lst: list = [x.capitalize() for x in init_lst]
    print(cap_lst)

    print(Colors.OKCYAN + "\nNew list of capitalized names only: ", end="")
    print(Colors.ENDC, end="")
    init_cap_lst: list = [x for x in init_lst if x.capitalize() == x]
    print(init_cap_lst)

    print(Colors.OKCYAN + "\nScore dict: ", end="" + Colors.ENDC)
    score_dict: dict = {name: random.randint(0, 1000) for name in init_cap_lst}
    print(score_dict)

    print(Colors.OKCYAN + "\nScore average is: ", end="" + Colors.ENDC)
    score_avg: float = sum(score_dict[x] for x in score_dict) / len(score_dict)
    print(round(score_avg, 1))

    print(Colors.OKCYAN + "\nHigh Scores: ", end="" + Colors.ENDC)
    high_score: dict = {x: score_dict[x] for x in score_dict
                        if score_dict[x] > score_avg}
    print(high_score)
