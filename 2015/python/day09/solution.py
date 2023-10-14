f = open("input.txt", "r")
values = f.read().split('\n')

distances = {}

all_locations = []
for value in values:
    locations, distance = value.split(' = ')
    location_1, location_2 = locations.split(' to ')

    distances[tuple(sorted((location_1, location_2)))] = int(distance)

    if location_1 not in all_locations:
        all_locations.append(location_1)
    
    if location_2 not in all_locations:
        all_locations.append(location_2)

print(all_locations)
# Now do a Dijkstra search

def find_path_with_criteria(curr_location, remaining_locations, curr_dist, check_func):
    if len(remaining_locations) == 0:
        return curr_dist

    min_distances = []
    for location in remaining_locations:
        location_pair = tuple(sorted((curr_location, location)))
        remaining_locations_copy = remaining_locations[:]
        remaining_locations_copy.remove(location)
        min_distances.append(find_path_with_criteria(location, remaining_locations_copy, distances[location_pair], check_func))
    
    return curr_dist + check_func(min_distances)

# Part 1 use float('inf'), part 2 use float('-inf')
dist = float('-inf')
for location in all_locations:
    locations_copy = all_locations[:]
    locations_copy.remove(location)

    # Part 1
    # dist = min([dist, find_path_with_criteria(location, locations_copy, 0, min)])

    # Part 2
    dist = max([dist, find_path_with_criteria(location, locations_copy, 0, max)])


print(dist)