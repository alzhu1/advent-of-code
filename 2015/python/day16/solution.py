f = open("input.txt", "r")
values = f.read().split('\n')

aunts = []

thing_dict = {
    'children': 0,
    'cats': 1,
    'samoyeds': 2,
    'pomeranians': 3,
    'akitas': 4,
    'vizslas': 5,
    'goldfish': 6,
    'trees': 7,
    'cars': 8,
    'perfumes': 9
}

# Tuple definition
# (children, cats, samoyeds, pomeranians, akitas, vizslas, goldfish, trees, cars, perfumes)
correct_aunt_things = (3, 7, 2, 3, 0, 0, 5, 3, 2, 1)

for i in range(len(values)):
    value = values[i].split(': ')[1:]
    value = tuple([v for x in value for v in x.split(', ')])

    num_things = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
    for idx in range(0, len(value), 2):
        num_things[thing_dict[value[idx]]] = int(value[idx+1])
    
    aunts.append(tuple(num_things))

filtered_aunts = []

# Part 2
cats_index = 1
trees_index = 7

pomeranians_index = 3
goldfish_index = 6
for i in range(len(aunts)):
    aunt = aunts[i]
    possible_aunt = True

    for j in range(len(aunt)):
        if aunt[j] == -1:
            continue

        elif j == cats_index or j == trees_index:
            if aunt[j] <= correct_aunt_things[j]:
                possible_aunt = False
                break

        elif j == pomeranians_index or j == goldfish_index:
            if aunt[j] >= correct_aunt_things[j]:
                possible_aunt = False
                break

        elif aunt[j] != correct_aunt_things[j]:
            possible_aunt = False
            break
    
    if possible_aunt:
        filtered_aunts.append((i, aunt))

print(filtered_aunts)