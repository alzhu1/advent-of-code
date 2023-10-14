const INPUT = 1352;

function isWall([x, y]) {
    // Treat negatives as a wall
    if (x < 0 || y < 0) {
        return true;
    }

    let sum = (x+y)*(x+y)+3*x+y + INPUT;

    let count = 0;
    while (sum > 0) {
        if ((sum & 1) > 0) {
            count++;
        }
        sum >>= 1;
    }

    return count % 2 === 1;
}

const visited = new Set();
let queue = [];
queue.push([[1, 1], 0]);
while (queue.length > 0) {
    const [coordinates, moves] = queue.shift();
    const coordinateString = JSON.stringify(coordinates);

    if (visited.has(coordinateString)) {
        continue;
    }
    visited.add(coordinateString);

    const [x, y] = coordinates;

    // Part 1
    // if (x === 31 && y === 39) {
    //     console.log('Fewest moves: ', moves);
    //     break;
    // }

    // Part 2
    if (moves === 50) {
        continue;
    }

    const possible = [[x, y+1], [x+1, y], [x, y-1], [x-1, y]];
    for (let p of possible) {
        if (!isWall(p)) {
            queue.push([p, moves + 1]);
        }
    }
}

// Part 2
console.log(visited.size);