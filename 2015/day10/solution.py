import sys

input_string = sys.argv[1]

# Part 1 is 40 times, part 2 is 50 times
for _ in range(50):
    counter = 0
    last_char = ""

    game_string = ""
    for i in range(len(input_string)):
        if i == 0:
            counter += 1
            last_char = input_string[i]
        elif last_char == input_string[i]:
            counter += 1
        else:
            game_string += str(counter) + last_char
            counter = 1
            last_char = input_string[i]
    
    game_string += str(counter) + last_char
    input_string = game_string

print(len(input_string))