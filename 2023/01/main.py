#!/usr/bin/env python3

import sys
import re

sum = 0
for line in sys.stdin:
    if not line:
        continue
    m = re.findall(r'\d', line)
    t = int(m[0] + m[-1])
    sum += t
    print(t)

print()
print(sum)
