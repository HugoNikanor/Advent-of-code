#!/usr/bin/env python3

import sys
import re

nums = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]



def parseint(x: str, rev=False) -> int:
    try:
        return int(x)
    except ValueError:
        if rev:
            lst = [''.join(reversed(s)) for s in nums]
        else:
            lst = nums
        return lst.index(x) + 1


sum = 0
for line in sys.stdin:
    if not line:
        continue
    rx = r'one|two|three|four|five|six|seven|eight|nine'
    m1 = re.findall(r'\d|' + rx, line)
    m2 = re.findall(r'\d|' + ''.join(reversed(rx)),
                    ''.join(reversed(line)))
    t = int(str(parseint(m1[0])) + str(parseint(m2[0], rev=True)))
    sum += t
    print(t, " ", line.strip())

print()
print(sum)
