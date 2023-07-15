// Binary solution attempt with big int, this is super slow in part 2

let binaryInput = BigInt(0b11011110011011101);
let currSize = 17;
const DISK_SIZE = 272;

// Part 2
// const DISK_SIZE = 35651584;

while (currSize < DISK_SIZE) {
    let copy = BigInt(0);
    for (let i = 0; i < currSize; i++) {
        const check = (binaryInput & (BigInt(1) << BigInt(i)));
        // console.log(check, check > BigInt(0));
        copy |= (check > BigInt(0) ? BigInt(0) : BigInt(1));

        if (i !== currSize - 1) {
            copy <<= BigInt(1);
        }
    }

    binaryInput = (binaryInput << BigInt(currSize + 1)) | (copy);
    currSize += (currSize + 1);

    console.log(currSize);
}

// Now find the checksum
let binaryChecksum = binaryInput >> BigInt(currSize - DISK_SIZE);
currSize = DISK_SIZE;

while (currSize % 2 === 0) {
    let checksumTemp = BigInt(0);

    for (let i = currSize - 2; i >= 0; i -= 2) {
        let comp = (binaryChecksum >> BigInt(i)) & BigInt(0b11);

        if (comp === BigInt(0) || comp === BigInt(3)) {
            checksumTemp |= BigInt(1);
        } else {
            checksumTemp |= BigInt(0);
        }

        if (i !== 0) {
            checksumTemp <<= BigInt(1);
        }
    }

    binaryChecksum = checksumTemp;
    currSize /= 2;
}

console.log(binaryChecksum.toString(2), currSize);