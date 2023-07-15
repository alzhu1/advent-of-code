const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

const messageLength = values[0].length;
const messageTracker = [];
for (let i = 0; i < messageLength; i++) {
    messageTracker.push({});
}

for (let value of values) {
    for (let i = 0; i < messageLength; i++) {
        const tracker = messageTracker[i];
        const c = value[i];

        if (!tracker[c]) {
            tracker[c] = 0;
        }
        tracker[c]++;
    }
}

let message = '';
for (let tracker of messageTracker) {
    // Part 1
    // let currCount = 0;

    // Part 2
    let currCount = Infinity;
    let currKey = '';
    Object.entries(tracker).forEach(([key, value]) => {
        // Part 1
        // if (value > currCount) {
        
        // Part 2
        if (value < currCount) {
            currCount = value;
            currKey = key;
        }
    });
    message += currKey;
}
console.log(message);