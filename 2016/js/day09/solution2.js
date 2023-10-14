const fs = require('fs');

const value = fs.readFileSync('input.txt', 'utf8')

// Should accept any string, and iterate over it
// Recursive calls occur on substring, and iteration index should move past the substring (in original string)
function stringRecurse(substring) {
    let idx = 0;
    let lengthCounter = 0;
    while (idx < substring.length) {
        if (substring[idx] !== '(') {
            lengthCounter++;
            idx++;
            continue;
        }

        // Encounter a marker, parse it out and recurse
        const markerEndIndex = substring.indexOf(')', idx);
        const [nextStringLength, repetitions] = substring.substring(idx + 1, markerEndIndex).split('x').map(x => parseInt(x));
    
        const nextString = substring.substring(markerEndIndex + 1, markerEndIndex + 1 + nextStringLength);
        lengthCounter += repetitions * stringRecurse(nextString);
        idx = markerEndIndex + 1 + nextStringLength;
    }
    return lengthCounter;
}

console.log(stringRecurse(value));