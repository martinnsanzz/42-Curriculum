#!/usr/bin/env python3

def print_plant_info(name: str, height: int, age: int) -> None:
    print("=== Welcome to My Garden ===")
    print(f"Plant: {name}")
    print(f"Height: {height}")
    print(f"Age: {age}", end="\n\n")
    print("=== End of Program ===")


if __name__ == '__main__':
    name = "Rose"
    height = 25
    age = 30
    print_plant_info(name, height, age)
