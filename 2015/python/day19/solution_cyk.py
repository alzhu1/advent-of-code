# TODO: at some point, implement cyk algorithm
# My solution was 195, btw

f = open("input.txt", "r")
values = f.read().split('\n')

# values = [
#     'H => HO',
#     'H => OH',
#     'O => HH',
#     'e => H',
#     'e => O',
#     '',
#     'HOHOHO'
# ]

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