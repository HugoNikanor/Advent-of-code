#!/usr/bin/env python

import math

arrival = int(input())
departures = [int(x) for x in input().split(',') if x != 'x']

wait_times = []

for departure in departures:
    wait_time = departure * math.ceil(arrival / departure) - arrival
    wait_times.append((wait_time, departure))
    # print(f'{departure} - {time}')

for (t, bus) in sorted(wait_times):
    print(f'{bus:3} - {t:3} minutes ({bus * t})')

