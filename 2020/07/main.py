import re

with open('input') as f:
    lines = f.readlines()

tree = {}

for line in lines:
    if m := re.match('(.*?) bags? contain (no other bags|((\d*) (.*) bags?[.,])+)', line):
        parent = m.group(1) # "light brown"
        if m.group(2) == 'no other bags':
            children = []
        else:
            # [('3', 'drab brown')]
            children = [m for m in re.findall('(\d*) ([^,.]*) bags?[,.] ?', m.group(2))]
        tree[parent] = children
    else:
        print("Error on line")
        print(line)
        break


# - 1 in both prints to remove the target bag

def find(tree, key, value):
    if key == value:
        return 1
    else:
        return sum(find(tree, child, value)
                for (_, child) in tree[key])

values = [find(tree, root, 'shiny gold')
    for root in tree]
print("Bag colors containing 'shiny gold' =",
        sum([1 for x in values if x != 0]) - 1)

def bag_count(tree, key):
    total = 1
    for (count, name) in tree[key]:
        total += int(count) * bag_count(tree, name)
    return total

print("Bags in the 'shiny gold' bag =", 
        bag_count(tree, 'shiny gold') - 1)
