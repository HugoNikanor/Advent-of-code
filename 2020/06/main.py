#!/usr/bin/env python

from functools import reduce

groups = []

try:
    while True:
        sets = []
        while line := input():
            sets.append(set(line))

        try:
            groups.append(reduce(lambda done, s: done.intersection(s), sets))
        except TypeError:
            groups.append({})
except EOFError:
    try:
        groups.append(reduce(lambda done, s: done.intersection(s), sets))
    except TypeError:
        groups.append({})

sizes = [len(g) for g in groups]
print(sizes)
print(sum(sizes))
