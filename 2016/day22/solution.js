const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

let nodes = [];

let smallest = Infinity;
for (let value of values) {
    const splitValue = value.split(' ').filter(x => x);
    if (splitValue[0][0] !== '/') {
        continue;
    }

    const [location, totalSize, used, available, usage] = splitValue;

    const [x, y] = location.split('-').map((v, i) => {
        if (i === 0) {
            return;
        }
        const unit = i === 1 ? 'x' : 'y';

        return parseInt(v.substring(v.indexOf(unit) + 1));
    }).filter(x => !isNaN(x));

    const [usedNum, availableNum] = [used, available].map(x => parseInt(x.substring(0, x.indexOf('T'))));

    nodes.push([[x, y], [usedNum, availableNum]]);

    smallest = Math.min(smallest, parseInt(totalSize.substring(0, totalSize.indexOf('T'))));
}

console.log(smallest);

// Sort by descending order of available space
nodes.sort((a, b) => b[1][1] - a[1][1]);

// console.log(nodes.slice(nodes.length - 10))

let viableCount = 0;
for (let i = 0; i < nodes.length; i++) {
    const [coords, usedAvailable] = nodes[i];
    if (usedAvailable[0] === 0) {
        continue;
    }

    for (let j = 0; j < nodes.length; j++) {
        if (i === j) {
            continue;
        }

        const [jCoords, jUsedAvailable] = nodes[j];

        if (jCoords[0] === 27 && jCoords[1] === 12 && jUsedAvailable[1] >= usedAvailable[0]) {
            viableCount++;
        }
    }
}
console.log(viableCount);