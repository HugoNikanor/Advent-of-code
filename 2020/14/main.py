#!/usr/bin/env python

import re

def parse_mask(mask):
    upmask = 0
    downmask = 1
    for digit in mask:
        downmask |= 1
        if digit == '1':
            upmask |= 1
        elif digit == '0':
            downmask ^= 1
        upmask <<= 1
        downmask <<= 1

    return upmask, downmask


mem = {}


with open('input') as f:
    upmask = 0
    downmask = 0
    for line in f:
        if m := re.match('^mask = (.*)', line):
            upmask, downmask = parse_mask(m.group(1))
            print()
            print(m.group(1))
            print(f'{bin(upmask)[2:]:>36}')
            print(f'{bin(downmask)[2:]:>36}')
        elif m := re.match('^mem\[(\d*)\] = (.*)', line):
            mem[int(m.group(1))] = downmask & (upmask | int(m.group(2)))

print(mem)
