code_coordinates = (2981, 3075)

offset = code_coordinates[1] - 1
row_at_first_col = code_coordinates[0] + offset

code_position = (row_at_first_col * (row_at_first_col - 1)) / 2 + 1
code_position += offset

code_position = int(code_position)

print(code_position)

code = 20151125
for _ in range(1, code_position):
    code = (code * 252533) % 33554393
print(code)