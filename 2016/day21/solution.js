const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

let starter = 'abcdefgh';

starter = starter.split('').map(c => c.charCodeAt(0));

for (let value of values) {
    const [verb, ...rest] = value.split(' ');
    if (verb === 'swap') {
        const positionBased = rest[0] === 'position';

        const pos1 = positionBased ? parseInt(rest[1]) : starter.indexOf(rest[1].charCodeAt(0));
        const pos2 = positionBased ? parseInt(rest[4]) : starter.indexOf(rest[4].charCodeAt(0));

        const temp = starter[pos1];
        starter[pos1] = starter[pos2];
        starter[pos2] = temp;

        // console.log(starter.reduce((acc, c) => `${acc}${String.fromCharCode(c)}`, ''));
    } else if (verb === 'rotate') {
        const rotateType = rest[0];

        let rotationAmount;
        if (rotateType === 'based') {
            rotationAmount = starter.indexOf(rest[5].charCodeAt(0));
            if (rotationAmount >= 4) {
                rotationAmount++;
            }
            rotationAmount++;
        } else {
            rotationAmount = parseInt(rest[1]);
            if (rest[0] === 'left') {
                rotationAmount *= -1;
            }
        }

        rotationAmount %= starter.length;
        starter = [...starter.slice(-rotationAmount), ...starter.slice(0, -rotationAmount)];
    } else if (verb === 'reverse') {
        const start = parseInt(rest[1]);
        const end = parseInt(rest[3]);

        starter = [...starter.slice(0, start), ...starter.slice(start, end + 1).reverse(), ...starter.slice(end + 1)];
    } else if (verb === 'move') {
        const start = parseInt(rest[1]);
        const final = parseInt(rest[4]);

        if (start < final) {
            starter = [...starter.slice(0, start), ...starter.slice(start + 1, final + 1), starter[start], ...starter.slice(final + 1)]
        } else if (start > final) {
            starter = [...starter.slice(0, final), starter[start], ...starter.slice(final, start), ...starter.slice(start + 1)];
        }
    }
    console.log(starter.reduce((acc, c) => `${acc}${String.fromCharCode(c)}`, ''));
}