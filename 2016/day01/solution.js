const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split(', ');

// 0 = N, 1 = E, 2 = S, 3 = W
const directions = [[0, 1], [1, 0], [0, -1], [-1, 0]];

let currPos = [0, 0];
let currDirection = 0;

const visited = new Set([`${currPos[0]}, ${currPos[1]}`]);
let firstVisit = false;

for (let value of values) {
    const directionChange = value.at(0);
    let blocks = parseInt(value.substring(1));

    if (directionChange === 'L') {
        currDirection -= 1;
        if (currDirection < 0) {
            currDirection = directions.length - 1;
        }
    } else if (directionChange === 'R') {
        currDirection = (currDirection + 1) % directions.length;
    }

    const [deltaX, deltaY] = directions[currDirection];

    while (blocks > 0) {
        currPos[0] += deltaX;
        currPos[1] += deltaY;

        if (!firstVisit && visited.has(`${currPos[0]}, ${currPos[1]}`)) {
            console.log('Visited: ', currPos);
            console.log('Double location distance: ', Math.abs(currPos[0]) + Math.abs(currPos[1]));
            firstVisit = true;
        } else {
            visited.add(`${currPos[0]}, ${currPos[1]}`);
        }

        blocks--;
    }
}

console.log(Math.abs(currPos[0]) + Math.abs(currPos[1]))