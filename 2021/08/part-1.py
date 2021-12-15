import sys

lefts = []
rights = []

for line in sys.stdin:
    left, right = line.split('|')
    ll = [item.strip() for item in left.split(' ') if item]
    rr = [item.strip() for item in right.split(' ') if item]
    lefts += ll
    rights += rr
    for item in rr:
        if len(item) in [2, 4, 3, 7]:
            print(f'\x1b[1;37m{item}\x1b[m ', end='')
        else:
            print(f'{item} ',end='')
    print()

print(lefts)
print(rights)

count = 0
for item in rights:
    if len(item) in [2, 4, 3, 7]:
        count += 1
print(count)
