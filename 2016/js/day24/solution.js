const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

// const values = [
//     '###########',
//     '#0.1.....2#',
//     '#.#######.#',
//     '#4.......3#',
//     '###########',
// ];
const NUM_POINTS = 8;

// Idea: do BFS on each number to create a smaller graph
// Then loop through each path (?) to find the smallest combination?

let grid = []

let row = 0;

const locationMap = new Map();

for (let value of values) {
    for (let i = 0; i < NUM_POINTS; i++) {
        const index = value.indexOf(i.toString());
        if (index !== -1) {
            locationMap.set(i, [index, row]);
        }
    }

    grid.push(value.split(''));
    row++;
}

// [from, to] => shortest path
const distanceMap = new Map();

function search(startPoint) {
    let queue = [[locationMap.get(startPoint), 0]];
    const visited = new Set();

    while (queue.length > 0) {
        const [coordinates, moves] = queue.shift();

        if (visited.has(JSON.stringify(coordinates))) {
            continue;
        }
        visited.add(JSON.stringify(coordinates));

        const [x, y] = coordinates;
        if (x !== y && grid[y][x] !== '#' && grid[y][x] !== '.') {
            // Found a point, mark it down
            const point = parseInt(grid[y][x]);
            const pair = [Math.min(startPoint, point), Math.max(startPoint, point)];
            if (!distanceMap.has(JSON.stringify(pair))) {
                distanceMap.set(JSON.stringify(pair), moves);
            }
        }

        const directions = [[x, y+1], [x+1, y], [x, y-1], [x-1, y]];
        for (let direction of directions) {
            const [nextX, nextY] = direction;

            if (nextY < 0 || nextY >= grid.length || nextX < 0 || nextX >= grid[nextY].length) {
                continue;
            }

            if (grid[nextY][nextX] === '#') {
                continue;
            }

            queue.push([direction, moves + 1]);
        }
    }
}

for (let i = 0; i < NUM_POINTS; i++) {
    search(i);
}

function recurse(currPoint, remainingPoints, currDistance, part2 = false) {
    // Part 2: must save 0 for last
    if (part2 && remainingPoints.length > 1 && !remainingPoints.includes(0)) {
        return Infinity;
    }

    if (remainingPoints.length === 0) {
        return currDistance;
    }

    let best = Infinity;
    for (let i = 0; i < remainingPoints.length; i++) {
        const nextPoints = [...remainingPoints.slice(0, i), ...remainingPoints.slice(i + 1)];
        const key = [Math.min(currPoint, remainingPoints[i]), Math.max(currPoint, remainingPoints[i])];
        best = Math.min(best, recurse(remainingPoints[i], nextPoints, currDistance + distanceMap.get(JSON.stringify(key)), part2));
    }
    return best;
}

// Part 1
console.log(recurse(0, [...Array(NUM_POINTS - 1).keys()].map(x => x+ 1), 0));

// Part 2
console.log(recurse(0, [...Array(NUM_POINTS).keys()], 0, true));