f = open("input.txt", "r")
values = f.read().split('\n')

mapping_dict = {}
oppo_mapping_dict = {}
count_dict = {}

starting_molecule = values[-1]
values = values[:len(values)-2]
longest_start_value = float('-inf')

for value in values:
    start, end = value.split(' => ')
    if start not in mapping_dict:
        mapping_dict[start] = []
    
    if end not in oppo_mapping_dict:
        oppo_mapping_dict[end] = []

    mapping_dict[start].append(end)
    oppo_mapping_dict[end].append(start)

    longest_start_value = max(longest_start_value, len(start))

depth_dict = {}

# NOTE: This greedy algorithm doesn't totally work
# Brute force solutions are below, realistcally should use CYK algorithm (based on online hints)
count = 0
while starting_molecule != 'e':
    for key, mapping in oppo_mapping_dict.items():
        value = mapping[0]

        if key in starting_molecule:
            starting_molecule = starting_molecule.replace(key, value, 1)

print(count)


# queue = [(starting_molecule, 0)]
# found = False
# while len(queue) > 0 or not found:
#     curr_string, depth = queue[0]
#     queue = queue[1:]

#     if curr_string in depth_dict:
#         continue

#     depth_dict[curr_string] = depth

#     if curr_string == 'e':
#         found = True
#         break
    
#     for key, oppo_map in oppo_mapping_dict.items():
#         # print("key and oppo map:", key, oppo_map)
#         for i in range(len(starting_molecule) - len(key) + 1):
#             substring = curr_string[i:i+len(key)]
#             if substring == key:
#                 # Check all mappings
#                 # molecule_copy = curr_string[:]
#                 pre_substring, post_substring = curr_string[:i], curr_string[i+len(key):]
#                 for mapping in oppo_map:
#                     full_string = pre_substring + mapping + post_substring
#                     if full_string not in depth_dict:
#                         queue.append((full_string, depth + 1))

# print(depth_dict['e'])
# print(queue)



def molecule_search(curr_string, depth, target):
    if curr_string in depth_dict:
        if depth_dict[curr_string] > depth:
            depth_dict[curr_string] = depth
        return depth_dict[curr_string]

    depth_dict[curr_string] = depth

    if curr_string == target:
        print("Matched target at depth:", depth)
        return depth

    if len(curr_string) >= len(target):
        return depth

    contains_mapping = False
    for key in mapping_dict.keys():
        if key in target:
            contains_mapping = True
            break
    
    if not contains_mapping:
        return depth

    min_depth = float('inf')
    for length in range(longest_start_value):
        for i in range(len(starting_molecule) - length):
            substring = curr_string[i:i+length+1]

            if substring in mapping_dict:
                # Check all mappings
                # molecule_copy = curr_string[:]
                pre_substring, post_substring = curr_string[:i], curr_string[i+length+1:]
                for mapping in mapping_dict[substring]:
                    full_string = pre_substring + mapping + post_substring
                    min_depth = min(min_depth, molecule_search(full_string, depth + 1, target))
    
    return min_depth

# Re-use depth dict?
def oppo_molecule_search(curr_string, depth, target):
    if curr_string in depth_dict:
        return depth_dict[curr_string]

    if curr_string == target:
        print("Matched target at depth:", depth)
        depth_dict[curr_string] = depth
        return depth

    for key in mapping_dict.keys():
        if key in target:
            contains_mapping = True
            break
    





    min_depth = float('inf')
    for length in range(longest_start_value):
        for i in range(len(starting_molecule) - length):
            substring = curr_string[i:i+length+1]

            if substring in mapping_dict:
                # Check all mappings
                # molecule_copy = curr_string[:]
                pre_substring, post_substring = curr_string[:i], curr_string[i+length+1:]
                for mapping in mapping_dict[substring]:
                    full_string = pre_substring + mapping + post_substring
                    min_depth = min(min_depth, molecule_search(full_string, depth + 1, target))
    
    depth_dict[curr_string] = min_depth
    return min_depth

# print(molecule_search('e', 0, starting_molecule))
# print(oppo_molecule_search(starting_molecule, 0, 'e'))

# Other idea: start with starting_molecule, and branch downwards
# print(oppo_mapping_dict.values())

