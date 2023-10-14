const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

const sequence = [];

const bannedSet = new Set([
    '0, 0',
    '0, 1',
    '0, 3',
    '0, 4',
    '1, 0',
    '1, 4',
    '3, 0',
    '3, 4',
    '4, 0',
    '4, 1',
    '4, 3',
    '4, 4'
]);

for (let value of values) {
    let currPos = [0, 2];

    for (let c of value) {
        let check = 0;

        switch (c) {
            case 'U':
                check = Math.min(currPos[1] + 1, 4);
                if (bannedSet.has(`${currPos[0]}, ${check}`)) {
                    check = currPos[1];
                }
                currPos[1] = check;
                break;
            case 'R':
                check = Math.min(currPos[0] + 1, 4);
                if (bannedSet.has(`${check}, ${currPos[1]}`)) {
                    check = currPos[0];
                }
                currPos[0] = check;
                break;
            case 'D':
                check = Math.max(currPos[1] - 1, 0);
                if (bannedSet.has(`${currPos[0]}, ${check}`)) {
                    check = currPos[1];
                }
                currPos[1] = check;
                break;
            case 'L':
                check = Math.max(currPos[0] - 1, 0);
                if (bannedSet.has(`${check}, ${currPos[1]}`)) {
                    check = currPos[0];
                }
                currPos[0] = check;
                break;
        }
    }
    sequence.push(currPos);
}

console.log(sequence);