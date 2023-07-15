const crypto = require('crypto');

input = process.argv[2];

let index = 0;
let password1 = '';
let password2 = ['', '', '', '', '' ,'' ,'' ,'']
let count = 0;

// Surprisingly, still slow even when using number/bit manipulation
while (count < 8) {
    const toHash = `${input}${index++}`;
    const hash = crypto.createHash('md5').update(toHash).digest();

    const buffer = hash.readUInt32BE(0);

    if ((buffer & 0xfffff000) === 0) {
        // Part 1
        // password1 += (hash[2] & 0x0f).toString(16);
        // count++;
        // console.log(password1);

        // Part 2
        const parsedIdx = (hash[2] & 0x0f);
        if (parsedIdx >= 0 && parsedIdx < 8 && password2[parsedIdx] === '') {
            password2[parsedIdx] = ((hash[3] & 0xf0) >> 4).toString(16);
            count++;
            console.log(password2);
        }
    }
}
console.log(password1);
console.log(password2.join(''))