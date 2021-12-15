population = [int(s) for s in input().split(',')]

for day in range(256 + 1):
    next_population = []
    # print(f"after {day:2} days: {population}")
    print(f"population day {day:2}: {len(population)}")
    for fish in population:
        if fish == 0:
            next_population.append(8) # new fish
            next_population.append(6) # reset ourselves
        else:
            next_population.append(fish - 1)
    population = next_population
