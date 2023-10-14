f = open("input.txt", "r")
values = f.read().split('\n')

values = list(sorted([int(x) for x in values]))

max_liters = 150

# Part 2 uses this
min_containers_used = float('inf')

# Key: tuple of len(values) mapping to each liter
# Value: number of liters
cases = {}

def check_liters(input_tuple):
    global min_containers_used

    if input_tuple in cases:
        return

    total_liters = 0
    for i in range(len(input_tuple)):
        if input_tuple[i] > 1:
            return
        total_liters += input_tuple[i] * values[i]
    
    if total_liters > max_liters:
        return

    if total_liters == max_liters:
        min_containers_used = min(min_containers_used, sum(input_tuple))
    
    for i in range(len(input_tuple)):
        check_liters(input_tuple[:i] + ((input_tuple[i] + 1),) + input_tuple[i+1:])

    cases[input_tuple] = total_liters

check_liters((0,) * len(values))

def match_liter_filter(pair):
    key, value = pair
    return value == max_liters and sum(key) == min_containers_used

print(len(dict(filter(match_liter_filter, cases.items()))))