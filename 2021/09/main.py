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
            # print('\x1b[1;37m', end='')
            low_points.append((x, y))
        # print(f'{p}\x1b[m', end='')
        # scad.append(f'translate([{x}, {y}, 0]) cube([1,1,{p}]);')
    print()

# print(sum(x + 1 for x in low_points))

# scad.append('')

# with open('output.scad', 'w') as f:
#     f.write('\n'.join(scad))

world_colors = [[0] * len(world[0]) for _ in world]

def neighbours_of(point):
    return [ point for point in [
        [point[0], point[1] + 1],
        [point[0], point[1] - 1],
        [point[0] - 1, point[1]],
        [point[0] + 1, point[1]],
    ] if 0 < point[0] < len(world[0]) - 1 and 0 < point[1] < len(world) - 1]

color = 1
counts = []
for point in low_points:
    color += 1
    # flood fill backwards from point
    world_colors[point[1]][point[0]] = color
    point_queue = [point]
    count = 0
    while point_queue:
        count += 1
        for neighbour in neighbours_of(point_queue.pop()):
            if world[neighbour[1]][neighbour[0]] == 9: continue
            if world[point[1]][point[0]] > world[neighbour[1]][neighbour[0]]: continue
            if world_colors[neighbour[1]][neighbour[0]] != 0: continue
            world_colors[neighbour[1]][neighbour[0]] = color
            point_queue.append(neighbour)
    counts.append((color, count))



def hsv_to_rgb(h, s, v):
    """
    0 ≤ h < 360
    0 ≤ s ≤ 1
    0 ≤ v ≤ 1
    """
    c = v * s
    x = c * (1 - abs((h / 60) % 2 - 1))
    m = v - c

    def cases(h):
        if h < 60: return (c, x, 0)
        elif h < 120: return (x, c, 0)
        elif h < 180: return (0, c, x)
        elif h < 240: return (0, x, c)
        elif h < 300: return (x, 0, c)
        else: return (c, 0, x)

    (rr, gg, bb) = cases(h)

    return ( 
            (rr + m) * 255,
            (gg + m) * 255,
            (bb + m) * 255
            )

for (y, row) in enumerate(world_colors):
    for (x, item) in enumerate(row):
        height = world[y][x]
        hh = height if height < 10 else ' '
        v = 1 - (height / 9)
        s = 1
        h = 360 * (item / (color + 1))
        r, g, b = hsv_to_rgb(h, s, v)
        r = int(r)
        g = int(g)
        b = int(b)
        # ESC[ 48;2;⟨r⟩;⟨g⟩;⟨b⟩ m Select RGB background color
        # print (f'\x1b[4{item}m \x1b[m', end='')
        # print(f'\x1b[48;2;{r};{g};{b}m{hh}\x1b[m', end='')
        print(f'\x1b[48;2;{r};{g};{b}m  \x1b[m', end='')
    print('')

from math import prod

print(prod(c for (_, c) in sorted(counts, key=lambda p: p[1], reverse=True)[:3]))
