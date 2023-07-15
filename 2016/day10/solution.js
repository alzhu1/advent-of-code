const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

const outputs = {};
const bots = {};

// Format: [value, bot to assign]
const valueAssignments = [];

// First instantiate the rules
for (let value of values) {
    const words = value.split(' ');
    if (words[0] === 'value') {
        valueAssignments.push([parseInt(words[1]), parseInt(words[5])]);
        continue;
    }
    const botNumber = parseInt(words[1]);
    const lowThing = words[5];
    const lowNumber = parseInt(words[6]);

    const highThing = words[10];
    const highNumber = parseInt(words[11]);

    bots[botNumber] = {
        values: [],
        low: {
            type: lowThing,
            num: lowNumber
        },
        high: {
            type: highThing,
            num: highNumber
        }
    };
}

function botRecurse(value, num, type) {
    if (type === 'output') {
        outputs[num] = value;
        return;
    }

    const { values, low, high } = bots[num];

    values.push(value);
    if (values.length == 2) {
        const [v1, v2] = values;
        const lower = Math.min(v1, v2);
        const higher = Math.max(v1, v2);

        // Part 1:
        if (lower === 17 && higher === 61) {
            console.log('Bot comparing these is: ', num);
        }

        botRecurse(lower, low.num, low.type);
        botRecurse(higher, high.num, high.type);

        values.length = 0;
    }
} 

for (let assignment of valueAssignments) {
    const [value, botNumber] = assignment;

    botRecurse(value, botNumber, 'bot');
}

// Part 2:
console.log('Output 0 * 1 * 2 = ', outputs[0] * outputs[1] * outputs[2]);