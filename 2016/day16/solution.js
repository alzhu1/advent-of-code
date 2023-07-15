let input = '11011110011011101';
// const DISK_SIZE = 272;

// Part 2
const DISK_SIZE = 35651584;

// First, dragon curve
while (input.length < DISK_SIZE) {
    let copy = '';
    for (let i = input.length - 1; i >= 0; i--) {
        copy += input[i] === '1' ? '0' : '1';
    }
    input = `${input}0${copy}`;
}

console.log('Finished input creation');

let checksum = input.substring(0, DISK_SIZE);

// Now checksum
while (checksum.length % 2 === 0) {
    let checksumTemp = '';

    for (let i = 0; i < checksum.length; i += 2) {
        if (checksum[i] === checksum[i+1]) {
            checksumTemp += '1';
        } else {
            checksumTemp += '0'
        }
    }
    checksum = checksumTemp;
}

console.log(checksum, checksum.length);