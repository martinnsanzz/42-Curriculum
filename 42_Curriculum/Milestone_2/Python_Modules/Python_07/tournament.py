#!/usr/bin/env python3


import ex0
import ex1
import ex2


class C:
    H = '\033[95m'  # Header
    B = '\033[94m'  # Blue
    C = '\033[96m'  # Cyan
    G = '\033[92m'  # Green
    W = '\033[93m'  # Warning
    F = '\033[91m'  # Fail
    E = '\033[0m'  # End
    Bo = '\033[1m'  # Bold
    U = '\033[4m'

    def msg(self, color: str, msg: str):
        print(getattr(self, color) + msg + C.E)


def display_opponents(opponents: list) -> None:
    list_opponents: list = []

    for x in opponents:
        creature = x[0].create_base()
        creature_list = (creature.name, x[1].strat_name)
        list_opponents.append("(" + "+".join(creature_list) + ")")
    C().msg("C", "Opponents:")
    print(f"    [ {", ".join(list_opponents)} ]\n")


def fight(creature_list: list, strat_list: list) -> None:
    C().msg("F", "\n * Battle *")
    print(creature_list[0].describe())
    C().msg("Bo", "  VS  ")
    print(creature_list[1].describe())
    C().msg("Bo", " Now fight !!")
    try:
        strat_list[0].act(creature_list[0])
        strat_list[1].act(creature_list[1])
    except Exception as error:
        C().msg("F", f"Battle error, aborting tournament: {error}")


def fight_multiple(creature_list: list, strat_list: list) -> None:
    fight([creature_list[0], creature_list[1]], [strat_list[0], strat_list[1]])
    fight([creature_list[0], creature_list[2]], [strat_list[0], strat_list[2]])
    fight([creature_list[1], creature_list[2]], [strat_list[1], strat_list[2]])


def battle_function(opponents: list) -> None:
    display_opponents(opponents)

    C().msg("Bo", "*** Tournament ***")
    print(f"{len(opponents)} opponents involved")
    creature_list = []
    strat_list = []
    for x in opponents:
        creature = x[0].create_base()
        creature_list.append(creature)
        strat_list.append(x[1])

    if len(creature_list) == 2:
        fight(creature_list, strat_list)
    else:
        fight_multiple(creature_list, strat_list)


if __name__ == "__main__":
    flame_factory = ex0.FlameFactory()
    aqua_factory = ex0.AquaFactory()
    heal_factory = ex1.HealingCreatureFactory()
    transform_factory = ex1.TransformCreatureFactory()

    normal_strat = ex2.NormalStrategy()
    aggresive_strat = ex2.AggresiveStrategy()
    defensive_strat = ex2.DefensiveStrategy()

    C().msg("H", "=== Tournument 0 (basic) ===")
    battle_function([(flame_factory, normal_strat),
                     (heal_factory, defensive_strat)])

    C().msg("H", "=== Tournument 1 (error) ===")
    battle_function([(flame_factory, aggresive_strat),
                     (heal_factory, defensive_strat)])

    C().msg("H", "=== Tournument 2 (Multiple) ===")
    battle_function([(aqua_factory, normal_strat),
                     (heal_factory, defensive_strat),
                     (transform_factory, aggresive_strat)])
