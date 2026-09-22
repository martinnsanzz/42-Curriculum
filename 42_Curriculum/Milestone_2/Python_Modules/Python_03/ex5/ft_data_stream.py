#!/usr/bin/env python3


import random
import typing


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


class Actions():
    def __init__(self) -> None:
        self.name_lst: list = [
            "bob",
            "alice",
            "dylan",
            "charlie"
        ]
        self.action_lst: list = [
            "run",
            "sleep",
            "grab",
            "move",
            "climb",
            "swim",
            "eat",
            "release"
        ]


def gen_event() -> typing.Generator[tuple[str, ...], None, None]:
    actions: Actions = Actions()
    while True:
        yield tuple((random.choice(actions.name_lst),
                     random.choice(actions.action_lst)))


def consume_event(tup_list: list) -> typing.Generator[tuple, None, None]:
    while True:
        random_pick: tuple = random.choice(tup_list)
        tup_list.remove(random_pick)
        yield random_pick


if __name__ == "__main__":
    print(Colors.HEADER + "=== Game Data Stream Processor ===" + Colors.ENDC)

    for i in range(0, 1000):
        tup: tuple[str, ...] = next(gen_event())
        print(Colors.BOLD + f"Event {i}: ", end="" + Colors.ENDC)
        print(f"Player {tup[0]} did action {tup[1]}")

    tup_list: list = list()
    for i in range(0, 10):
        tup_list.append(tuple(next(gen_event())))
    print(Colors.OKCYAN + "\nBuilt list of 10 events: ", end="" + Colors.ENDC)
    print(tup_list, end="\n\n")

    while tup_list:
        removed_item: tuple = next(consume_event(tup_list))
        print(Colors.OKCYAN + "Got event from list: ", end="" + Colors.ENDC)
        print(removed_item)
        print(Colors.OKGREEN + "Remains in list: ", end="" + Colors.ENDC)
        print(tup_list)
