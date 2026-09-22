#!/usr/bin/env python3


import random


ACHIEVEMENT_LIST = [
    "First Blood",
    "Hoarder",
    "Unstoppable",
    "MacGyver",
    "Glutton for Punishment",
    "Pacifist",
    "Speed Runner",
    "Cartographer",
    "Myth Buster",
    "Team Player",
    "Overkill",
    "Sneak Thief",
    "Loremaster",
    "Fashion Victim",
    "Breaking the Matrix",
    "Eco-Friendly",
    "Penny Pincher",
    "Animal Lover",
    "Insomniac",
    "Serious Dedication"
]


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


def gen_player_achievements() -> set:
    num_elements: int = random.randint(1, len(ACHIEVEMENT_LIST))
    rand_set: set = set(random.sample(ACHIEVEMENT_LIST, k=num_elements))

    return rand_set


def display_player_info(player: set, player_name: str) -> None:
    print(Colors.OKGREEN + f"Player {player_name}: " + Colors.ENDC, end="")
    print(player)


if __name__ == "__main__":
    alice: set = gen_player_achievements()
    bob: set = gen_player_achievements()
    charlie: set = gen_player_achievements()
    dylan: set = gen_player_achievements()

    print(Colors.HEADER + "=== Achievement Tracker System ===", end="\n\n")
    display_player_info(alice, "Alice")
    display_player_info(bob, "Bob")
    display_player_info(charlie, "Charlie")
    display_player_info(dylan, "Dylan")

    print(Colors.OKBLUE + "\nAll distinct achievements: ", end="")
    print(Colors.ENDC + f"{set.union(alice, bob, charlie, dylan)}", end="\n\n")

    print(Colors.OKBLUE + "\nCommon achievements: " + Colors.ENDC, end="")
    print(set.intersection(alice, bob, charlie, dylan), end="\n\n")

    print(Colors.OKCYAN + "Only Alice has: " + Colors.ENDC, end="")
    print(set.difference(alice, bob, charlie, dylan))
    print(Colors.OKCYAN + "Only Bob has: " + Colors.ENDC, end="")
    print(set.difference(bob, alice, charlie, dylan))
    print(Colors.OKCYAN + "Only Charlie has: " + Colors.ENDC, end="")
    print(set.difference(charlie, bob, alice, dylan))
    print(Colors.OKCYAN + "Only Dylan has: " + Colors.ENDC, end="")
    print(set.difference(dylan, bob, charlie, alice))

    print(Colors.OKCYAN + f"\nAlice '{len(alice)}' is missing: ", end="")
    print(Colors.ENDC + f"{set.difference(set(ACHIEVEMENT_LIST), alice)}")
    print(Colors.OKCYAN + f"Bob '{len(bob)}' is missing: ", end="")
    print(Colors.ENDC + f"{set.difference(set(ACHIEVEMENT_LIST), bob)}")
    print(Colors.OKCYAN + f"Charlie '{len(charlie)}' is missing: ", end="")
    print(Colors.ENDC + f"{set.difference(set(ACHIEVEMENT_LIST), charlie)}")
    print(Colors.OKCYAN + f"Dylan '{len(dylan)}' is missing: ", end="")
    print(Colors.ENDC + f"{set.difference(set(ACHIEVEMENT_LIST), dylan)}")
