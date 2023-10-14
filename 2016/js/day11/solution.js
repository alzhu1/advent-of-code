// NOTE: it's too annoying to do string parsing for these inputs lol, just doing it manually

function getCacheKey(state) {
    const stateCopy = [...state]

    const mapping = new Map();
    let cacheKey = `currFloor:${stateCopy[0]}|`;

    let pairs = [];
    for (let i = 1; i <= 4; i++) {
        const chips = (stateCopy[i] & 0xff00) >> 8;
        const generators = stateCopy[i] & 0x00ff;

        for (let c = 0; c < 8; c++) {
            const checker = 1 << c;
            if ((chips & checker) > 0) {
                // Found a chip at floor i, check if mapping contains type
                if (!mapping.has(c)) {
                    mapping.set(c, i);
                } else {
                    pairs.push([i, mapping.get(c)]);
                }
            }
        }


        for (let g = 0; g < 8; g++) {
            const checker = 1 << g;
            if ((generators & checker) > 0) {
                // Found a generator at floor i, check if mapping contains type
                if (!mapping.has(g)) {
                    mapping.set(g, i);
                } else {
                    pairs.push([mapping.get(g), i]);
                }
            }
        }

    }

    return `${cacheKey}${JSON.stringify(pairs)}`;
}

// const testState = [
//     1,
//     0b00000011_00000000,
//     0b00000000_00000010,
//     0b00000000_00000001,
//     0
// ];

const testState = [
    1, // Floor index
    (0b00000011 << 8) | (0b00000011), // State of floor 1
    (0b00011000 << 8) | (0b00011100), // State of floor 2
    (0b00000100 << 8) | (0b00000000), // State of floor 3
    0 // State of floor 4
];

// Part 2:
testState[1] |= ((0b01100000 << 8) | (0b01100000))

const cache = new Map();

let s = 0;
let queue = [[testState, 0]];
let lastLowestMove = -1;

// BFS is the way to go... other hints from https://www.reddit.com/r/adventofcode/comments/5hoia9/2016_day_11_solutions/
while (queue.length > 0) {
    const [state, moves] = queue.shift();

    const cacheKey = getCacheKey(state);
    if (cache.has(cacheKey)) {
        continue;
    }
    cache.set(cacheKey, moves);

    if (moves > lastLowestMove) {
        console.log('First occurence of moves: ', moves, ', iters: ', s, 'curr queue length: ', queue.length);
        lastLowestMove = moves;
    }
    s++;

    const currFloor = state[0];

    // Success: when all chips and generators in floors 0, 1, and 2 are empty
    if (currFloor === 4 && state[1] === 0 && state[2] === 0 && state[3] === 0) {
        console.log('Success at: ', moves);
        break;
    }

    // Check for failure states:
    let fail = false;
    for (let i = 1; i <= 4; i++) {
        const chips = (state[i] & 0xff00) >> 8;
        const generators = state[i] & 0x00ff;

        if (generators !== 0) {

            for (let bitIdx = 0; bitIdx < 8; bitIdx++) {
                const checker = 1 << bitIdx;

                if ((chips & checker) > 0 && (generators & checker) === 0) {
                    // This means chip has no corresponding generator, fail
                    fail = true;
                    break;
                }
            }
        }

        if (fail) {
            break;
        }
    }
    if (fail) {
        continue;
    }

    // If a lower floor is empty, we don't need to use it
    let emptyLowerFloors = true;
    for (let i = 0; i < currFloor; i++) {
        const chips = (state[i] & 0xff00) >> 8;
        const generators = state[i] & 0x00ff;
        if (chips !== 0 || generators !== 0) {
            emptyLowerFloors = false;
            break;
        }
    }

    const chips = (state[currFloor] & 0xff00) >> 8;
    const generators = state[currFloor] & 0x00ff;

    const chipStrings = []
    const generatorStrings = []
    for (let i = 0; i < 8; i++) {
        const checker = 1 << i;

        if ((chips & checker) > 0) {
            chipStrings.push(`c${i}`);
        }

        if ((generators & checker) > 0) {
            generatorStrings.push(`g${i}`);
        }
    }

    const combined = [...chipStrings, ...generatorStrings];
    const combos = [...combined.flatMap(
        (v, i) => {
            return combined.slice(i+1).map(w => {
                // Don't bother actions where chip and generator don't match
                if (v[0] !== w[0] && v[1] !== w[1]) {
                    return;
                }
                return v + ' ' + w
            })
        }
    ),
    ...chipStrings,
    ...generatorStrings].filter(x => x);

    for (let combo of combos) {
        const items = combo.split(' ');

        let item1, item2;
        item1 = items[0];
        if (items.length === 2) {
            item2 = items[1];
        }

        let chipsCopy = chips;
        let generatorsCopy = generators;

        const item1Index = parseInt(item1[1]);
        if (item1[0] === 'c') {
            chipsCopy ^= (1 << item1Index);
        } else {
            generatorsCopy ^= (1 << item1Index);
        }

        if (item2) {
            const item2Index = parseInt(item2[1]);
            if (item2[0] === 'c') {
                // chipsCopy.splice(chipsCopy.indexOf(parseInt(item2[1])), 1);
                chipsCopy ^= (1 << item2Index);
            } else {
                // generatorsCopy.splice(generatorsCopy.indexOf(parseInt(item2[1])), 1);
                generatorsCopy ^= (1 << item2Index);
            }
        }

        if (currFloor - 1 >= 1 && !emptyLowerFloors) {
            const stateCopy = [...state];
            stateCopy[0] = currFloor - 1;

            const currFloorItems = (chipsCopy << 8) | generatorsCopy;
            stateCopy[currFloor] = currFloorItems;

            // const { chips: chipsNext, generators: generatorsNext } = stateCopy[currFloor - 1];
            if (item1[0] === 'c') {
                // chipsNext.push(parseInt(item1[1]));
                stateCopy[currFloor - 1] |= (1 << 8 << item1Index);
            } else {
                // generatorsNext.push(parseInt(item1[1]));
                stateCopy[currFloor - 1] |= (1 << item1Index);
            }
    
            if (item2) {
                const item2Index = parseInt(item2[1]);
                if (item2[0] === 'c') {
                    // chipsNext.push(parseInt(item2[1]));
                    stateCopy[currFloor - 1] |= (1 << 8 << item2Index);
                } else {
                    // generatorsNext.push(parseInt(item2[1]));
                    stateCopy[currFloor - 1] |= (1 << item2Index);
                }
            }
            queue.push([stateCopy, moves + 1]);
        }

        if (currFloor + 1 < 5) {
            const stateCopy = [...state];
            stateCopy[0] = currFloor + 1;

            const currFloorItems = (chipsCopy << 8) | generatorsCopy;
            stateCopy[currFloor] = currFloorItems;

            if (item1[0] === 'c') {
                stateCopy[currFloor + 1] |= (1 << 8 << item1Index);
            } else {
                stateCopy[currFloor + 1] |= (1 << item1Index);
            }
    
            if (item2) {
                const item2Index = parseInt(item2[1]);
                if (item2[0] === 'c') {
                    // chipsNext.push(parseInt(item2[1]));
                    stateCopy[currFloor + 1] |= (1 << 8 << item2Index);
                } else {
                    // generatorsNext.push(parseInt(item2[1]));
                    stateCopy[currFloor + 1] |= (1 << item2Index);
                }
            }

            queue.push([stateCopy, moves + 1]);
        }
    }
}
