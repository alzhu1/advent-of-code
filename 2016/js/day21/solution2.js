const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n').reverse();

let starter = 'fbgdceah';

starter = starter.split('').map(c => c.charCodeAt(0));

const rotationAmounts = starter.map((_, i) => {
    // return value = the final index you end up at
    let rotationAmount = i >= 4 ? 2 : 1;
    return (i + (i + rotationAmount)) % starter.length;
});

const finalRotationToStart = new Map();
rotationAmounts.forEach((v, i) => {
    if (!finalRotationToStart.has(v)) {
        finalRotationToStart.set(v, []);
    }
    finalRotationToStart.get(v).push(i);
});

let starterList = [starter];

// Reversing
for (let value of values) {
    let nextStarter = [];

    for (let starter of starterList) {
        const [verb, ...rest] = value.split(' ');
        if (verb === 'swap') {
            const positionBased = rest[0] === 'position';

            const pos1 = positionBased ? parseInt(rest[1]) : starter.indexOf(rest[1].charCodeAt(0));
            const pos2 = positionBased ? parseInt(rest[4]) : starter.indexOf(rest[4].charCodeAt(0));

            const min = Math.min(pos1, pos2);
            const max = Math.max(pos1, pos2);

            nextStarter.push([...starter.slice(0, min), starter[max], ...starter.slice(min + 1, max), starter[min], ...starter.slice(max + 1)])
        } else if (verb === 'rotate') {
            const rotateType = rest[0];

            let rotationAmount;
            if (rotateType === 'based') {
                const currIndex = starter.indexOf(rest[5].charCodeAt(0));
                const reverseRotationAmount = finalRotationToStart.get(currIndex);

                for (let prevIndex of reverseRotationAmount) {
                    // prevIndex represents possible spots for currIndex to have come from
                    rotationAmount = prevIndex - currIndex;

                    nextStarter.push([...starter.slice(-rotationAmount), ...starter.slice(0, -rotationAmount)]);
                }
                continue;
            } else {
                rotationAmount = parseInt(rest[1]);
                if (rest[0] === 'left') {
                    rotationAmount *= -1;
                }
            }

            rotationAmount *= -1;

            rotationAmount %= starter.length;
            nextStarter.push([...starter.slice(-rotationAmount), ...starter.slice(0, -rotationAmount)]);
        } else if (verb === 'reverse') {
            const start = parseInt(rest[1]);
            const end = parseInt(rest[3]);

            nextStarter.push([...starter.slice(0, start), ...starter.slice(start, end + 1).reverse(), ...starter.slice(end + 1)]);
        } else if (verb === 'move') {
            const start = parseInt(rest[4]);
            const final = parseInt(rest[1]);

            if (start < final) {
                nextStarter.push([...starter.slice(0, start), ...starter.slice(start + 1, final + 1), starter[start], ...starter.slice(final + 1)])
            } else if (start > final) {
                nextStarter.push([...starter.slice(0, final), starter[start], ...starter.slice(final, start), ...starter.slice(start + 1)]);
            }
        }
    }

    starterList = nextStarter;
}

console.log(starterList.map(starter => starter.reduce((acc, c) => `${acc}${String.fromCharCode(c)}`, '')));