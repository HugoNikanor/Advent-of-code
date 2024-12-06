#!/usr/bin/env python3

import re
import sys

mem = sys.stdin.read()

print(sum(int(m[1]) * int(m[2])
          for m in
          re.finditer(r'mul\((\d+),(\d+)\)', mem)))
