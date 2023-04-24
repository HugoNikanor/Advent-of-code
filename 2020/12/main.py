#!/usr/bin/env python

class Pos:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.facing = (1, 0)

    def rotate(self, lr, degrees):
        angle = degrees // 90
        if lr == 'l':
            for _ in range(angle):
                self.facing = {
                        (1, 0): (0, 1),
                        (0, 1): (-1, 0),
                        (-1, 0): (0, -1),
                        (0, -1): (1, 0),
                        }[self.facing]
        else:
            self.rotate('l', 360 - degrees)

    def move(self, direction, distance):
        if direction == 'F':
            v = self.facing
        else:
            v = (-self.facing[0], -self.facing[1])

        self.x += v[0] * distance
        self.y += v[1] * distance

    def slide(self, direction, distance):
        v = { 'N': (0, 1), 'S': (0, -1), 'E': (1, 0), 'W': (-1, 0) }[direction]
        self.x += v[0] * distance
        self.y += v[1] * distance


try:
    p = Pos()
    while True:
        line = input()

        print(line, end='');

        action = line[0]
        num = int(line[1:])

        if action in ['L', 'R']:
            p.rotate(action, num)
        elif action in ['F', 'B']:
            p.move(action, num)
        else:
            p.slide(action, num)

        print(f'\tx = {p.x}, y = {p.y}\t|x| + |y| = {abs(p.x) + abs(p.y)}')
except EOFError:
    pass
