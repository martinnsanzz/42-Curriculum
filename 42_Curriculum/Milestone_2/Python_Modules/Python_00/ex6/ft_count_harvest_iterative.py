def ft_count_harvest_iterative():
    next_harvest = int(input("Days until harvest: "))
    for x in range(1, (next_harvest + 1)):
        print(f"Day {x}")
    print("Harvest time!")
