def harvest_count(day, next_harvest):
    if (day > next_harvest):
        print("Harvest time!")
    else:
        print(f"Day {day}")
        harvest_count(day + 1, next_harvest)


def ft_count_harvest_recursive():
    next_harvest = int(input("Days until harvest: "))
    harvest_count(1, next_harvest)
