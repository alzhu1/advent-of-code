const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

const aInAscii = 'a'.charCodeAt(0);
const zInAscii = 'z'.charCodeAt(0);

let idTotal = 0;
for (let value of values) {
    let index = value.indexOf('[');
    const checksum = value.substring(index + 1).split(']')[0];

    const checksumIndex = index;
    while (value.at(index) !== '-') {
        index--;
    }

    const id = value.substring(index + 1, checksumIndex);

    const nameParts = {};
    const name = value.substring(0, index);
    name.split('-').forEach(x => {
        for (let c of x) {
            if (!nameParts[c]) {
                nameParts[c] = 0;
            }
            nameParts[c]++;
        }
    });

    const sortedNameParts = []
    for (let part in nameParts) {
        sortedNameParts.push([part, nameParts[part]]);
    }
    sortedNameParts.sort((a, b) => {
        if (b[1] == a[1]) {
            return a[0].localeCompare(b[0]);
        }
        return b[1] - a[1];
    });

    const correctChecksum = sortedNameParts.reduce((accumulator, namePart, index) => {
        if (index > 4) {
            return accumulator;
        }
        return `${accumulator}${namePart[0]}`;
    }, '');

    if (correctChecksum === checksum) {
        const parsedId = parseInt(id);
        idTotal += parsedId;

        // Part 2
        const shiftAmount = parsedId % 26;

        let finalName = '';
        for (let i = 0; i < name.length; i++) {
            if (name.charAt(i) === '-') {
                finalName += '-';
                continue;
            }

            let charCode = name.charCodeAt(i) + shiftAmount;
            if (charCode > zInAscii) {
                charCode = aInAscii + (charCode - zInAscii - 1);
            }
            finalName += String.fromCharCode(charCode);
        }
        console.log('Final name: ', finalName, ', Sector ID: ', parsedId);
    }
}

// Part 1
console.log('ID total: ', idTotal);