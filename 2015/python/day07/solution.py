f = open("input.txt", "r")
values = f.read().split('\n')

wire_dict = {}
unprocessed = []

def process(instruction):
    operation, destination = instruction.split(' -> ')
    wire_dict[destination] = operation

    # Part 2 change
    if destination == 'b':
        wire_dict[destination] = '46065'

for value in values:
    processed = process(value)

processed_wire_dict = {}

def get_full_circuit(wire):
    if wire.isnumeric():
        return int(wire)

    if wire in processed_wire_dict:
        return processed_wire_dict[wire]

    operation = wire_dict[wire]

    split_operation = operation.split(' ')

    if len(split_operation) == 1:
        processed_wire_dict[wire] = get_full_circuit(split_operation[0])
    elif split_operation[0] == 'NOT':
        processed_wire_dict[wire] = ~(get_full_circuit(split_operation[1]))
    elif split_operation[1] == 'AND':
        processed_wire_dict[wire] = get_full_circuit(split_operation[0]) & get_full_circuit(split_operation[2])
    elif split_operation[1] == 'OR':
        processed_wire_dict[wire] = get_full_circuit(split_operation[0]) | get_full_circuit(split_operation[2])
    elif split_operation[1] == 'LSHIFT':
        processed_wire_dict[wire] = get_full_circuit(split_operation[0]) << get_full_circuit(split_operation[2])
    elif split_operation[1] == 'RSHIFT':
        processed_wire_dict[wire] = get_full_circuit(split_operation[0]) >> get_full_circuit(split_operation[2])

    return processed_wire_dict[wire]

print(get_full_circuit('a'))