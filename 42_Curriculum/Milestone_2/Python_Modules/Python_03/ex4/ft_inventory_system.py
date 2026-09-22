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


class CustomError(Exception):
    def __init__(self, msg: str) -> None:
        super().__init__(msg)


def check_format(param: str) -> None:
    if ":" not in param:
        raise CustomError(f"Correct format <item_name>:<quantity>: '{param}'")
    tmp_list = param.split(":")
    if not len(tmp_list) == 2:
        raise CustomError(f"Format can't have more than one ':': '{param}'")
    if tmp_list[0] == '' or tmp_list[1] == '':
        raise CustomError(f"Format must be <item_name>:<quantity>: '{param}'")
    if not str.isnumeric(tmp_list[1]):
        raise CustomError(f"Quantity must be a positive number: '{param}'")


def parse_params(argv: list) -> dict:
    inventory: dict = {}
    for arg in argv[1:len(argv)]:
        try:
            check_format(arg)
            item = arg.split(":")
            if item[0] in inventory:
                print(Colors.WARNING, end="")
                print(f"Discarding duplicate item: '{item[0]}'" + Colors.ENDC)
            else:
                inventory.update({item[0]: item[1]})
        except CustomError as error:
            print(Colors.FAIL + f"Error - {error}" + Colors.ENDC)
    return inventory


def print_perc_of_item(inventory: dict, total: int) -> None:
    for item in inventory:
        print(f"Item {item} represents: ", end="")
        percent: float = (int(inventory[item]) / total) * 100
        print(Colors.OKGREEN + f"{round(percent, 1)}%" + Colors.ENDC)


def print_most_least(inventory: dict) -> None:
    tmp_max = list(inventory.values())[0]
    most_abundant = list(inventory.keys())[0]
    for item in inventory:
        if int(inventory[item]) > int(tmp_max):
            most_abundant = item
            tmp_max = inventory[item]

    print(Colors.OKCYAN + "\nItem most abundant: ", end="")
    print(Colors.ENDC, end="")
    print(f"{most_abundant} with quantity {int(inventory[most_abundant])}")

    tmp_min = list(inventory.values())[0]
    least_abundant = list(inventory.keys())[0]
    for item in inventory:
        if int(inventory[item]) < int(tmp_min):
            least_abundant = item
            tmp_min = inventory[item]

    print(Colors.OKCYAN + "Item least abundant: ", end="")
    print(Colors.ENDC, end="")
    print(f"{least_abundant} with quantity {int(inventory[least_abundant])}")


if __name__ == "__main__":
    print(Colors.HEADER, end="")
    print("=== Inventory System Analysis ===" + Colors.ENDC)

    inventory: dict = parse_params(sys.argv)

    if not len(inventory) == 0:
        print(Colors.OKCYAN + "\nGot inventory: " + Colors.ENDC, end="")
        print(inventory)

        print(Colors.OKCYAN + "Item list: " + Colors.ENDC, end="")
        print(list(inventory.keys()))
        print(Colors.OKCYAN + "Total quantity of the ", end="")
        print(f"{len(inventory)} items: " + Colors.ENDC, end="")
        total_quantity = sum(int(x) for x in list(inventory.values()))
        print(total_quantity, end="\n\n")

        print_perc_of_item(inventory, total_quantity)

        print_most_least(inventory)
    else:
        print(Colors.WARNING + "Inventory is empty, no objects provided...")
        print(Colors.ENDC, end="")

    inventory.update({"pickaxe": "1"})
    print(Colors.OKCYAN + "\nUpdated inventory: " + Colors.ENDC, end="")
    print(inventory)
