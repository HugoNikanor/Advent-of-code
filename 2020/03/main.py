#!/usr/bin/env python

from functools import reduce

class RepeatingGrid:
    class RepeatingRow:
        def __init__(self, grid, row):
            self.grid = grid
            self.row = row

        def __getitem__(self, col):
            return self.row[col % len(self.row)]

    def __init__(self, grid):
        self.grid = grid
        self.rows = len(grid)

    def __getitem__(self, row):
        return RepeatingGrid.RepeatingRow(self, self.grid[row])



def read_grid(port):
    return RepeatingGrid([line for line in port.read().split('\n')
                            if line])

with open('input') as f:
    grid = read_grid(f)

def check_slope(dx, dy):
    column = 0
    counts = { '.': 0, '#': 0 }
    for row in range(0, grid.rows, dy):
        counts[grid[row][column]] += 1
        column += dx
    return counts

counts = \
[check_slope(dx, dy) for (dx, dy)
        in [(1, 1),
            (3, 1),
            (5, 1),
            (7, 1),
            (1, 2)]]

print(reduce(lambda x, y: x * y, [count['#'] for count in counts]))
