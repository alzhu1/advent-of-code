import numpy as np

f = open("input.txt", "r")
values = f.read().split('\n')

lights = np.zeros([100, 100])
steps = 100

# Part 2 uses this
corner_set = { (0, 0), (0, len(lights[0]) - 1), (len(lights) - 1, 0), (len(lights) - 1, len(lights[0]) - 1) }

for i in range(len(values)):
    value = values[i]

    for j in range(len(value)):
        lights[i, j] = 1 if value[j] == '#' else 0

for i, j in corner_set:
    lights[i, j] = 1

for _ in range(steps):
    lights_copy = np.copy(lights)

    for i in range(len(lights)):
        row = lights[i]

        for j in range(len(row)):
            if (i, j) in corner_set:
                lights_copy[i, j] = 1
                continue
            
            min_i, max_i = max(0, i - 1), min(len(lights) - 1, i + 1)
            min_j, max_j = max(0, j - 1), min(len(row) - 1, j + 1)

            lit_neighbors = np.sum(lights[min_i:max_i+1, min_j:max_j+1]) - lights[i, j]

            if lights[i, j] == 1:
                lights_copy[i, j] = 1 if lit_neighbors == 2 or lit_neighbors == 3 else 0
            else:
                lights_copy[i, j] = 1 if lit_neighbors == 3 else 0

    lights = lights_copy

print(np.sum(lights))