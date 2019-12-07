import sys
from graphviz import Digraph

orbits = {}
orbiting = {}

dot = Digraph()

for line in map(str.rstrip, sys.stdin):
    center, orbiter = line.split(')')

    dot.node(center)
    dot.node(orbiter)
    dot.edge(center, orbiter)

    orbits[orbiter] = orbits.get(orbiter, [])
    orbits[center] = orbits.get(center, []) + [orbiter]

    orbiting[orbiter] = center

def f(node, i):
    return i + sum (f(orbits[name], i + 1) for name in node)

# part1
print("oribts =", f(orbits["COM"], 0))

#part2

def get_path(node):
    if node == "COM":
        return []
    else:
        return [node] + get_path(orbiting[node])


you = get_path(orbiting["YOU"])[::-1]
san = get_path(orbiting["SAN"])[::-1]
while you[0] == san[0]:
    you = you[1:]
    san = san[1:]

print("hops YOU->SAN =", len(you) + len(san))

dot.attr(rankdir='LR')

with open("graph.dot", "w") as f:
    f.write(dot.source)

# > 1558
#
# A-B-C
#   | 
#   D
#   
