const fs = require('fs');

const instructions = fs.readFileSync('input.txt', 'utf8').split('\n');

const registers = {
    a: 0,
    b: 0,
    c: 0,
    d: 0
};

// Part 2:
registers.c = 1;

let programCounter = 0;
while (programCounter >= 0 && programCounter < instructions.length) {
    const instruction = instructions[programCounter];
    const [opCode, ...rest] = instruction.split(' ');

    switch (opCode) {
        case 'cpy': {
            const [value, r] = rest;

            if (!isNaN(value)) {
                // Is a number
                registers[r] = parseInt(value);
            } else {
                registers[r] = registers[value];
            }

            break;
        }

        case 'inc': {
            registers[rest[0]]++;
            break;
        }

        case 'dec': {
            registers[rest[0]]--;
            break;
        }

        case 'jnz': {
            const [r, offset] = rest;
            if (registers[r] !== 0) {
                programCounter += parseInt(offset) - 1;
            }
            break;
        }
    }

    programCounter++;
}

console.log(registers);