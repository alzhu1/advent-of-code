const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

let ranges = [];
for (let value of values) {
    ranges.push(value.split('-').map(x => parseInt(x)));
}

ranges.sort((a, b) => a[0] - b[0]);

let allowCount = 0;
let min = Infinity;

let [_, maxCheck] = ranges[0];
for (let i = 1; i < ranges.length; i++) {
    let [low, high] = ranges[i];

    if (maxCheck +1  < low) {
        min = Math.min(min, maxCheck + 1);
        allowCount += (low - maxCheck - 1);
    }

    maxCheck = Math.max(maxCheck, high);
}
console.log('Smallest allowed IP: ', min, ', allow count: ', allowCount);