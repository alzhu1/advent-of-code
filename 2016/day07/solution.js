const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

// Part 1
function abbaCheck(substring) {
    const outsideMatch = substring.charAt(0) === substring.charAt(3);
    const insideMatch = substring.charAt(1) === substring.charAt(2);
    const consecutiveNoMatch = substring.charAt(0) !== substring.charAt(1);

    return outsideMatch && insideMatch && consecutiveNoMatch;
}

let count = 0;
for (let value of values) {
    let tlsSupport = false;
    let inBracket = false;

    for (let i = 0; i < value.length - 3; i++) {
        if (value.charAt(i + 3) === '[') {
            inBracket = true;
            continue
        }

        if (value.charAt(i) === ']') {
            inBracket = false;
            continue
        }
        const hasAbba = abbaCheck(value.substring(i, i + 4));

        if (hasAbba) {
            tlsSupport = true;
            if (inBracket) {
                tlsSupport = false;
                break;
            }
        }
    }

    if (tlsSupport) {
        count++;
    }
}

console.log(count);

// Part 2
function abaOrbabCheck(substring) {
    return substring.charAt(0) === substring.charAt(2) && substring.charAt(0) !== substring.charAt(1);
}

function getReversedAba(aba) {
    return `${aba[1]}${aba[0]}${aba[1]}`;
}

count = 0;
for (let value of values) {
    let abaSet = new Set();
    let babSet = new Set();
    let sslSupport = false;
    let inBracket = false;

    for (let i = 0; i < value.length - 2; i++) {
        if (value.charAt(i) === '[') {
            inBracket = true;
            continue
        }

        if (value.charAt(i) === ']') {
            inBracket = false;
            continue
        }

        const substring = value.substring(i, i + 3);
        const hasAbaOrBab = abaOrbabCheck(substring);
        if (hasAbaOrBab) {
            if (inBracket) {
                abaSet.add(substring);
            } else {
                babSet.add(substring);
            }
        }
    }
    
    abaSet.forEach(aba => {
        if (babSet.has(getReversedAba(aba))) {
            sslSupport = true;
            return;
        }
    });

    if (sslSupport) {
        count++;
    }
}

console.log(count);