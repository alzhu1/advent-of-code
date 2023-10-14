const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');
// const values = fs.readFileSync('input2.txt', 'utf8').split('\n');

const WALL_CUTOFF = 200;

let grid = [];
let emptyLocation = [0, 0];
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

    const usedNum = parseInt(used.substring(0, used.indexOf('T')));

    // Y first then X
    if (y >= grid.length) {
        grid.push([]);
    }

    if (usedNum > WALL_CUTOFF) { // Wall
        grid[y].push('#');
    } else if (usedNum === 0) { // Empty
        emptyLocation[0] = x;
        emptyLocation[1] = y;
        grid[y].push('_');
    } else { // Moveable
        grid[y].push('.');
    }
}
grid[0][grid[0].length - 1] = 'G';
const numRows = grid.length;
const numCols = grid[0].length;

// This is basically a sliding block puzzle, with some "holes"
// The node with 0 used space === open space
// Need to use A* search? Or could just try BFS

function getGridCopy(grid) {
    return grid.map(x => {
        return [...x];
    });
}

let queue = [[grid, emptyLocation, 0]];

const set = new Set();

while (queue.length > 0) {
    const [gridState, currLoc, moves] = queue.shift();

    if (set.has(JSON.stringify(gridState))) {
        continue;
    }
    set.add(JSON.stringify(gridState));

    const [currX, currY] = currLoc;
    if (gridState[currY][currX - 1] === 'G') {
        console.log('math it out: ', moves);

        // TODO: One optimization is A* search
        // Heuristic would probably be G's distance from 0, 0
        // And also maybe _'s distance to G at a lower weight?

        // Math answer: once G and _ have swapped and are on first row,
        // Just calculate min number of moves to move G closer to 0, 0
        // It should be 5 moves, multiply that by distance to 0, 0
        console.log('Answer: ', moves + (numCols - 2) * 5);
        break;
    }

    const nextLocations = [[currX, currY+1], [currX+1, currY], [currX, currY-1], [currX-1, currY]];

    for (let nextLoc of nextLocations) {
        const [nextX, nextY] = nextLoc;

        if (nextY < 0 || nextY >= numRows || nextX < 0 || nextX >= numCols) {
            continue;
        }

        if (gridState[nextY][nextX] === '#') {
            continue;
        }

        let gridCopy = getGridCopy(gridState);
        let temp = gridCopy[currY][currX];
        gridCopy[currY][currX] = gridCopy[nextY][nextX];
        gridCopy[nextY][nextX] = temp;

        queue.push([gridCopy, nextLoc, moves + 1]);
    }
}