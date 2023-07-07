f = open("input.txt", "r")
values = f.read().split('\n')

literal_length = 0
mem_length = 0
reencoded_length = 0
for value in values:
    literal_length += len(value)
    mem_length += len(bytes(value, 'utf-8').decode('unicode_escape')) - 2
    reencoded_length += (len(value) + value.count('"') + value.count('\\') + 2)

print("Part 1:\n")
print(literal_length)
print(mem_length)
print(literal_length - mem_length)

print("Part 2:\n")
print(reencoded_length)
print(literal_length)
print(reencoded_length - literal_length)