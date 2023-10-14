const crypto = require('crypto');

const hashStarter = 'qtetzkpl';
const pathSymbols = ['U', 'D', 'L', 'R'];

let queue = [[[0, 0], '']];

let longest = 0;

while (queue.length > 0) {
    const [coordinates, path] = queue.shift();
    const [x, y] = coordinates;

    if (x === 3 && y === 3) {
        // console.log('Path: ', path);

        // Part 2: do not break
        longest = Math.max(longest, path.length);
        console.log('Longest: ', longest, ', queue length: ', queue.length);
        continue;
        // break;
    }

    const [upDown, leftRight] = crypto.createHash('md5').update(`${hashStarter}${path}`).digest();
    // console.log(coordinates, crypto.createHash('md5').update(`${hashStarter}${path}`).digest());
    const openDoors = [upDown, leftRight].flatMap(v => {
        return [((v >> 4) & 0xf) > 0xa, (v & 0xf) > 0xa];
    })

    const directions = [[x, y-1], [x, y+1], [x-1, y], [x+1, y]];
    for (let i = 0; i < 4; i++) {
        const direction = directions[i];
        const [dx, dy] = direction;
        
        if (dx >= 0 && dx < 4 && dy >= 0 && dy < 4 && openDoors[i]) {
            queue.push([direction, `${path}${pathSymbols[i]}`]);
        }
    }
}
