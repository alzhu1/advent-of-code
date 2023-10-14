f = open("input.txt", "r")
values = f.read().split('\n')

mapping_dict = {}
count_dict = {}

starting_molecule = values[-1]
values = values[:len(values)-2]
longest_start_value = float('-inf')

for value in values:
    start, end = value.split(' => ')
    if start not in mapping_dict:
        mapping_dict[start] = []
    mapping_dict[start].append(end)

    longest_start_value = max(longest_start_value, len(start))

for length in range(longest_start_value):
    for i in range(len(starting_molecule) - length):
        substring = starting_molecule[i:i+length+1]

        if substring in mapping_dict:
            # Check all mappings
            molecule_copy = starting_molecule[:]
            pre_substring, post_substring = molecule_copy[:i], molecule_copy[i+length+1:]
            for mapping in mapping_dict[substring]:
                full_string = pre_substring + mapping + post_substring
                if full_string not in count_dict:
                    count_dict[full_string] = 1

print(len(count_dict))
