const fs = require('fs');

let instructions = fs.readFileSync('input.txt', 'utf8').split('\n');

const registers = {
    a: 12,
    b: 0,
    c: 0,
    d: 0
};
let programCounter = 0;

// Part 2: Looks like this is calculating a! up to the tgl instruction, just start there
registers.a = [...Array(registers.a).keys()].map(x => x + 1).reduce((acc, v) => acc * v, 1);
registers.b = 1;
registers.c = 2;
programCounter = 16;

let tempPc = programCounter;
while (tempPc < instructions.length) {
    toggle(tempPc);
    tempPc += 2;
}

// registers.c = 1;

function toggle(toggleIndex) {
    if (toggleIndex >= 0 && toggleIndex < instructions.length) {
        let toggleInstruction = instructions[toggleIndex].split(' ');

        // 1-arg (account for opcode)
        if (toggleInstruction.length === 2) {
            const isInc = toggleInstruction[0] === 'inc';
            toggleInstruction[0] = isInc ? 'dec' : 'inc';
        } else if (toggleInstruction.length === 3) {
            // 2-arg
            const isJnz = toggleInstruction[0] === 'jnz';
            toggleInstruction[0] = isJnz ? 'cpy' : 'jnz';
        }
        instructions[toggleIndex] = toggleInstruction.join(' ');
    }
}

let iters = 0;

while (programCounter >= 0 && programCounter < instructions.length) {
    const instruction = instructions[programCounter];
    const [opCode, ...rest] = instruction.split(' ');

    switch (opCode) {
        case 'cpy': {
            const [value, r] = rest;

            if (isNaN(r)) {
                if (!isNaN(value)) {
                    // Is a number
                    registers[r] = parseInt(value);
                } else {
                    registers[r] = registers[value];
                }
            }

            break;
        }

        case 'inc': {
            if (isNaN(rest[0])) {
                registers[rest[0]]++;
            }
            break;
        }

        case 'dec': {
            if (isNaN(rest[0])) {
                registers[rest[0]]--;
            }
            break;
        }

        case 'jnz': {
            const [r, offset] = rest;

            const pcOffset = isNaN(offset) ? registers[offset] : parseInt(offset);
            const notZero = (isNaN(r) && registers[r] !== 0) || (!isNaN(r) && parseInt(r) !== 0);

            if (notZero) {
                programCounter += pcOffset - 1;
            }
            break;
        }

        case 'tgl': {
            const toggleIndex = registers[rest[0]] + programCounter;
            console.log(registers, programCounter);
            toggle(toggleIndex);

            if (toggleIndex === programCounter) {
                console.log('No more toggles, this is instruction set now');
                console.log('PC (before moving to next instruction): ', programCounter);
                console.log(instructions);
            }
            break;
        }
    }

    iters++;
    // if (iters % 1000000 === 0) {
    //     console.log('PC: ', programCounter);
    //     console.log(registers);
    //     console.log(instructions)
    // }

    programCounter++;
}

console.log(instructions);
console.log(registers);