const fs = require('fs');

const values = fs.readFileSync('input.txt', 'utf8').split('\n');

const grid = [];
for (let i = 0; i < 6; i++) {
    grid.push(new Array(50).fill(0));
}

for (let value of values) {
    const parts = value.split(' ');
    if (parts[0] === 'rect') {
        const [width, height] = parts[1].split('x').map(x => parseInt(x));
        for (let h = 0; h < height; h++) {
            for (let w = 0; w < width; w++) {
                grid[h][w] = 1;
            }
        }

        // grid.forEach(x => console.log(JSON.stringify(x)));
        continue;
    }

    // Should be rotate otherwise, check second part
    const [axis, selection, _, amount] = parts.slice(1);
    let parsedAmount = parseInt(amount);
    const selectedAxis = parseInt(selection.split('=')[1]);

    if (axis === 'row') {
        let row = [...grid[selectedAxis]];
        parsedAmount %= row.length;

        for (let i = 0; i < parsedAmount; i++) {
            const a = row.pop();
            row.unshift(a);
        }
        grid[selectedAxis] = row;

        // grid.forEach(x => console.log(JSON.stringify(x)));
    } else if (axis === 'column') {
        let col = [];
        parsedAmount %= grid.length;

        for (let i = 0; i < grid.length; i++) {
            col.push(grid[i][selectedAxis]);
        }

        for (let i = 0; i < parsedAmount; i++) {
            const a = col.pop();
            col.unshift(a);
        }

        for (let i = 0; i < grid.length; i++) {
            grid[i][selectedAxis] = col[i];
        }

        // grid.forEach(x => console.log(JSON.stringify(x)));
    }
}

// Part 1
const sum = grid.reduce((accumulator, row) => {
    return row.reduce((accumulator2, pixel) => {
        return accumulator2 + pixel;
    }, accumulator);
}, 0);
console.log(sum);

// Part 2 parsing
grid.forEach(row => {
    let rowString = '';
    row.forEach(pixel => {
        rowString += (pixel == 1 ? '*' : ' ');
    });
    console.log(rowString);
});