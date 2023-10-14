const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

let possible = 0;
for (let value of values) {
    const [a, b, c] = value.trim().split(' ').filter(x => x).map(x => parseInt(x.trim()));

    if (a + b > c && a + c > b && b + c > a) {
        possible++;
    }
}
console.log(possible);