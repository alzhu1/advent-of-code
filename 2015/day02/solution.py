import sys

f = open("input.txt", "r")
values = f.read().split('\n')

# Wrapping
solution = 0
for input_value in values:
    if input_value == '':
        continue

    l_str, w_str, h_str = input_value.split('x')
    l, w, h = int(l_str), int(w_str), int(h_str)

    surface_area = 2*(l*w + w*h + l*h)

    dimension_list = [l, w, h]
    min_1 = min(dimension_list)
    dimension_list.remove(min_1)
    min_2 = min(dimension_list)

    solution += surface_area + (min_1 * min_2)

print(solution)

# Ribbon
solution = 0
for input_value in values:
    if input_value == '':
        continue

    l_str, w_str, h_str = input_value.split('x')
    l, w, h = int(l_str), int(w_str), int(h_str)

    perimeters = [l + w, l + h, w + h]
    small_perimeter = min(perimeters)

    solution += 2*small_perimeter + (l * w * h)

print(solution)
