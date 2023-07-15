const fs = require('fs');

const value = fs.readFileSync('input.txt', 'utf8')

// const value = 'X(8x2)(3x3)ABCY';

let i = 0;
let finalString = '';
while (i < value.length) {
    if (value[i] !== '(') {
        finalString += value[i];
        i++;
        continue;
    }

    // Need to handle marker case
    const markerEndIndex = value.indexOf(')', i);
    const [nextStringLength, repetitions] = value.substring(i + 1, markerEndIndex).split('x').map(x => parseInt(x));

    const nextString = value.substring(markerEndIndex + 1, markerEndIndex + 1 + nextStringLength);
    finalString += nextString.repeat(repetitions);

    i = markerEndIndex + 1 + nextStringLength;
}

console.log(finalString.length);