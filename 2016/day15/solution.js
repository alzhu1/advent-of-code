// Another one where typing the input is much easier than parsing

let discs = [
    // [numPositions, startPos]
    [13, 1],
    [19, 10],
    [3, 2],
    [7, 1],
    [5, 3],
    [17, 5],

    // Part 2
    [11, 0]
];

// Start testing with first possible 0 time
let t = discs[0][0] - discs[0][1];

while (true) {
    let checkTime = t;
    let succeeded = true;

    for (let disc of discs) {
        const [numPositions, startPos] = disc;
        const currPos = (startPos + checkTime) % numPositions;

        // Successful time if the pos is 0, otherwise try next one
        if (currPos === 0) {
            checkTime++;
        } else {
            succeeded = false;
            break;
        }
    }

    if (succeeded) {
        break;
    }

    // Increment by intervals of first disc num positions (to guarantee first 0)
    t += discs[0][0];
}

// Account for the extra second from t being at disc 1 already
console.log(t - 1);