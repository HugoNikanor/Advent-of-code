ships = [int(x) for x in input().split(',')]

costs = []

for pos in range(max(ships)):
    total_cost = 0
    for ship in ships:
        total_cost += abs(ship - pos)
    costs.append((pos, total_cost))

pos, cost = sorted(costs, key=lambda p: p[1])[0]

print(f'Position {pos}, costing {cost}')
