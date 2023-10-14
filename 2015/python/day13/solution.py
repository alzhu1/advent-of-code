f = open("input.txt", "r")
values = f.read().split('\n')

happy_dict = {}
all_people = []
for value in values:
    rest, person_2 = value.split(' happiness units by sitting next to ')
    person_2 = person_2[:len(person_2) - 1]

    person_1, rest = rest.split(' would ')
    action, number = rest.split(' ')

    if person_1 not in all_people:
        all_people.append(person_1)
    
    if person_2 not in all_people:
        all_people.append(person_2)

    if action == 'gain':
        happy_dict[(person_1, person_2)] = int(number)
    elif action == 'lose':
        happy_dict[(person_1, person_2)] = -int(number)

# Part 2 addition
for person in all_people:
    happy_dict[('You', person)] = 0
    happy_dict[(person, 'You')] = 0

all_people.append('You')

def place_guests(curr_arrangement, remaining_people):
    if len(remaining_people) == 0:
        # Calculate happiness by taking pairs
        total_happiness = 0
        for i in range(-1, len(curr_arrangement) - 1):
            person_1, person_2 = curr_arrangement[i], curr_arrangement[i+1]
            total_happiness += happy_dict[(person_1, person_2)] + happy_dict[(person_2, person_1)]
        return total_happiness
    
    # Call place_guests at each moment
    max_happiness = float('-inf')
    for person in remaining_people:
        curr_arrangement_copy = curr_arrangement[:]
        remaining_people_copy = remaining_people[:]

        curr_arrangement_copy.append(person)
        remaining_people_copy.remove(person)

        max_happiness = max(max_happiness, place_guests(curr_arrangement_copy, remaining_people_copy))
    
    return max_happiness

print(place_guests([], all_people))