# Basically just count the number of ( and ), subtract it
import sys

input_string = sys.argv[1]
value = 0

for i in range(len(sys.argv[1])):
    char = sys.argv[1][i]
    if char == '(':
        value = value + 1
    elif char == ')':
        value = value - 1
        if value == -1:
            print("Basement at position " + str(i + 1))

print(value)
