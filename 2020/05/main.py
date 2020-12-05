t = {
        'F': '0',
        'B': '1',
        'L': '0',
        'R': '1'
}

ids = []

try:
    while line := input():
        row = eval("0b" + ''.join(t[x] for x in line[:7]))
        col = eval("0b" + ''.join(t[x] for x in line[-3:]))
        # print(row, col)
        ids.append(row * 8 + col)
except EOFError:
    pass

# print(sorted(ids))
for (i, e) in zip(sorted(ids), range(min(ids), max(ids) + 1)):
    if i != e:
        print(e)
        break
print("max(ids) =", max(ids))
