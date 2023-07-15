const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

let sideCounter = 0
let possible = 0;
let tempTriangles = [[], [], []];
for (let value of values) {
    const [t1, t2, t3] = value.trim().split(' ').filter(x => x).map(x => parseInt(x.trim()));
    tempTriangles[0].push(t1);
    tempTriangles[1].push(t2);
    tempTriangles[2].push(t3);

    sideCounter++;

    if (sideCounter == 3) {
        sideCounter = 0;
        possible += checkTriangle(tempTriangles[0]);
        possible += checkTriangle(tempTriangles[1]);
        possible += checkTriangle(tempTriangles[2]);
        tempTriangles = [[], [], []];
    }
}

function checkTriangle([a, b, c]) {
    return (a + b > c && a + c > b && b + c > a) ? 1 : 0;
}

console.log(possible);