// let input = '^^^^......^...^..^....^^^.^^^.^.^^^^^^..^...^^...^^^.^^....^..^^^.^.^^...^.^...^^.^^^.^^^^.^^.^..^.^';

let input = '.^^.^.^^^^';
let safeCount = input.split('').reduce((acc, v) => {
    return v === '.' ? acc + 1 : acc;
}, 0)
const startCount = safeCount;

const rows = 126;

const map = new Map();
map.set(input, [0, safeCount]);

for (let i = 2; i <= rows; i++) {
    if (input[1] === '.') {
        safeCount++;
    }
    let temp = `${input[1]}`;

    for (let c = 0; c < input.length - 2; c++) {
        const safe = input[c] === input[c + 2];
        if (safe) {
            safeCount++;
            temp += '.';
        } else {
            temp += '^';
        }
    }
    temp += input[input.length - 2];
    if (input[input.length - 2] === '.') {
        safeCount++;
    }

    // TODO: One optimization is to find a repeated value, then do math to get rest

    // if (map.has(temp)) {
    //     // We found this already, time to do math
    //     console.log('Found at ', i, temp, map.get(temp));
    //     const [index, count] = map.get(temp);

    //     const numInBetween = i - index;

    //     const safeInBetween = count - startCount;
    //     const multiples = Math.floor(rows / numInBetween - 1);
    //     safeCount += safeInBetween * multiples;
    //     i += (i - index) * multiples;
    //     console.log(i);
    // } else {
    //     map.set(temp, [i, safeCount]);
    // }

    input = temp;
}

console.log(safeCount);