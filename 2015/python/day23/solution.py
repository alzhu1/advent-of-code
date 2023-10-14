f = open("input.txt", "r")
instructions = f.read().split('\n')

instruction_pointer = 0
registers = {
    # Part 2 sets a to 1
    'a': 1,
    'b': 0
}

# hlf r => r = r/2
# tpl r => r = r * 3
# inc r => r += 1
# jmp offset => go to instruction_pointer + offset
# jie r, offset => if r % 2 == 0: go to instruction_pointer + offset
# jio r, offset => if r == 1: go to instruction_pointer + offset

while instruction_pointer >= 0 and instruction_pointer < len(instructions):
    split_instruction = instructions[instruction_pointer].split(' ')

    if split_instruction[0] == 'hlf':
        registers[split_instruction[1]] = int(registers[split_instruction[1]] / 2)
    elif split_instruction[0] == 'tpl':
        registers[split_instruction[1]] *= 3
    elif split_instruction[0] == 'inc':
        registers[split_instruction[1]] += 1
    elif split_instruction[0] == 'jmp':
        instruction_pointer += int(split_instruction[1]) - 1
    else:
        # Handle jie and jio inputs
        register_name = split_instruction[1].split(',')[0]
        offset = int(split_instruction[2])

        if split_instruction[0] == 'jie' and registers[register_name] % 2 == 0:
            instruction_pointer += offset - 1
        elif split_instruction[0] == 'jio' and registers[register_name] == 1:
            instruction_pointer += offset - 1

    instruction_pointer += 1

print(registers)