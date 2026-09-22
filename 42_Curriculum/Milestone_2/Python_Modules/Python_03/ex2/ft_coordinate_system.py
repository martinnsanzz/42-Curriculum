#!/usr/bin/env python3


import math


class CustomError(Exception):
    def __init__(self, msg: str = "Unknow plant error") -> None:
        super().__init__(msg)


def check_tupple(coordinates: tuple) -> None:
    for x in coordinates:
        try:
            float(x)
        except Exception:
            raise CustomError(f"Error on paramameter '{x}'")

    if not len(coordinates) == 3:
        raise CustomError("Format must be 'x,y,z'")


def get_player_pos() -> tuple:
    coordinates: tuple = ()
    while not coordinates:
        try:
            print("Enter new coordinates as float in format 'x,y,z':", end="")
            user_input = input(" ")
            coordinates = tuple(user_input.split(","))
            check_tupple(coordinates)
        except CustomError as error:
            print(error)
            coordinates = ()
    return (tuple(float(x) for x in coordinates))


def display_coordinates(coordinates: tuple) -> None:
    print("It includes: ", end="")
    print(f"X = {coordinates[0]}, ", end="")
    print(f"Y = {coordinates[1]}, ", end="")
    print(f"Z = {coordinates[2]}")


def calculate_distance(pos_1: tuple,
                       pos_2: tuple = (0, 0, 0)) -> float:
    distance_x = (pos_2[0] - pos_1[0]) ** 2
    distance_y = (pos_2[1] - pos_1[1]) ** 2
    distance_z = (pos_2[2] - pos_1[2]) ** 2
    total_distance = math.sqrt(distance_x + distance_y + distance_z)
    return (total_distance)


if __name__ == "__main__":
    print("=== Game Coordinate System ===")

    print("\nGet a first set of coordinates")
    tuple_1: tuple = get_player_pos()
    print(f"Got first tuple: {tuple_1}")
    display_coordinates(tuple_1)
    print(f"Distance to center {round(calculate_distance(tuple_1), 4)}")

    print("\nGet a second set of coordinates")
    tuple_2: tuple = get_player_pos()
    print("Distance between the 2 sets of coordinates: ", end="")
    print(round(calculate_distance(tuple_1, tuple_2), 4))
