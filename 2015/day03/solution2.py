visit_dict = {}

f = open("input.txt", "r")
values = f.read()

# Visit on start by both santas
visit_dict[(0, 0)] = 2

robo_santa = False
santa_start_pos = (0, 0)
robo_santa_start_pos = (0, 0)

for value in values:
    start_pos = robo_santa_start_pos if robo_santa else santa_start_pos

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

    # Set
    if robo_santa:
        robo_santa_start_pos = start_pos
    else:
        santa_start_pos = start_pos
    
    robo_santa = not robo_santa

# All visited houses
print(len(visit_dict))