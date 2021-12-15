import sys

world = []

for line in sys.stdin:
    world.append([10] + [int(x) for x in line.strip()] + [10])

width = len(world[0]) - 2 
height = len(world)

border = [[10] * (width + 2)]
world = border + world + border

print(width, height)
print(world)

scad = []

low_points = []
for y in range(1, height + 1):
    for x in range(1, width + 1):
        p = world[y][x]
        if p < world[y + 1][x] and p < world[y - 1][x] and p < world[y][x + 1] and p < world[y][x - 1]:
            print('\x1b[1;37m', end='')
            low_points.append(p)
        print(f'{p}\x1b[m', end='')
        scad.append(f'translate([{x}, {y}, 0]) cube([1,1,{p}]);')
    print()

print(sum(x + 1 for x in low_points))

scad.append('')

with open('output.scad', 'w') as f:
    f.write('\n'.join(scad))
