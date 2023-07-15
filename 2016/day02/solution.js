const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

const sequence = [];
for (let value of values) {
    let currPos = [1, 1];

    for (let c of value) {
        switch (c) {
            case 'U':
                currPos[1] = Math.min(currPos[1] + 1, 2);
                break;
            case 'R':
                currPos[0] = Math.min(currPos[0] + 1, 2);
                break;
            case 'D':
                currPos[1] = Math.max(currPos[1] - 1, 0);
                break;
            case 'L':
                currPos[0] = Math.max(currPos[0] - 1, 0);
                break;
        }
    }
    sequence.push(currPos);
}

console.log(sequence);