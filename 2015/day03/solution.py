visit_dict = {}

f = open("input.txt", "r")
values = f.read()

# Visit on start
start_pos = (0, 0)
visit_dict[start_pos] = 1

for value in values:
    # Move
    if value == '^':
        start_pos = (start_pos[0], start_pos[1] + 1)
    elif value == '>':
        start_pos = (start_pos[0] + 1, start_pos[1])
    elif value == 'v':
        start_pos = (start_pos[0], start_pos[1] - 1)
    elif value == '<':
        start_pos = (start_pos[0] - 1, start_pos[1])

    # Visit
    if start_pos not in visit_dict:
        visit_dict[start_pos] = 0
    
    visit_dict[start_pos] += 1

# All visited houses
print(len(visit_dict))

# Filter out values with only 1
# def multi_visit(pair):
#     key, value = pair
#     return True if value >= 1 else False

# filtered_visit_dict = dict(filter(multi_visit, visit_dict.items()))
# print(len(filtered_visit_dict))