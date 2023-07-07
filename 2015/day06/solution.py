import numpy as np

f = open("input.txt", "r")
values = f.read().split('\n')

lights = np.zeros([1000, 1000])

# Format is "x1,y1 through x2,y2"
def get_params(rest):
    x1, y1 = [int(v) for v in rest[0].split(',')]
    x2, y2 = [int(v) for v in rest[2].split(',')]
    return [(x1, y1), (x2, y2)]

def turn_on(rest):
    (x1, y1), (x2, y2) = get_params(rest)

    min_x, min_y = min(x1, x2), min(y1, y2)
    max_x, max_y = max(x1, x2), max(y1, y2)

    # Part 1
    # lights[min_y:max_y + 1, min_x:max_x + 1] = 1

    # Part 2
    lights[min_y:max_y + 1, min_x:max_x + 1] += 1

def turn_off(rest):
    (x1, y1), (x2, y2) = get_params(rest)

    min_x, min_y = min(x1, x2), min(y1, y2)
    max_x, max_y = max(x1, x2), max(y1, y2)

    # Part 1
    # lights[min_y:max_y + 1, min_x:max_x + 1] = 0

    # Part 2
    lights[min_y:max_y + 1, min_x:max_x + 1] -= 1
    lights[min_y:max_y + 1, min_x:max_x + 1] = np.clip(lights[min_y:max_y + 1, min_x:max_x + 1], 0, None)

def toggle(rest):
    (x1, y1), (x2, y2) = get_params(rest)

    min_x, min_y = min(x1, x2), min(y1, y2)
    max_x, max_y = max(x1, x2), max(y1, y2)

    # Part 1
    # lights[min_y:max_y + 1, min_x:max_x + 1] = np.logical_not(lights[min_y:max_y + 1, min_x:max_x + 1])

    # Part 2
    lights[min_y:max_y + 1, min_x:max_x + 1] += 2

for value in values:
    split_values = value.split(' ')

    if split_values[0] == 'toggle':
        toggle(split_values[1:])
    elif split_values[0] == 'turn' and split_values[1] == 'on':
        turn_on(split_values[2:])
    elif split_values[0] == 'turn' and split_values[1] == 'off':
        turn_off(split_values[2:])

print(np.sum(lights))