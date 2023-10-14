const fs = require('fs');

const instructions = fs.readFileSync('input.txt', 'utf8').split('\n');

function simulate(a) {
    const registers = {
        a,
        b: 0,
        c: 0,
        d: 0
    };

    let programCounter = 0;
    let lastTransmit = 1;

    let incorrect = false;
    let checks = 0;
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

                const pcOffset = isNaN(offset) ? registers[offset] : parseInt(offset);
                const notZero = (isNaN(r) && registers[r] !== 0) || (!isNaN(r) && parseInt(r) !== 0);
    
                if (notZero) {
                    programCounter += pcOffset - 1;
                }
                break;
            }

            case 'out': {
                const transmit = isNaN(rest[0]) ? registers[rest[0]] : parseInt(rest[0]);
                const correctNextValue = lastTransmit === 1 ? 0 : 1;

                if (transmit !== correctNextValue) {
                    incorrect = true;
                } else {
                    lastTransmit = transmit;
                    checks++;
                }
            }
        }

        // Just a probabilistic check, technically not guaranteed
        if (incorrect) {
            return false;
        } else if (checks >= 100) {
            return true;
        }

        programCounter++;
    }
}

let i = 1;
while (true) {
    if (simulate(i)) {
        break;
    }
    i++;
}

console.log(i);