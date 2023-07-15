const numElves = 3014387;
let elves = [...Array(numElves).keys()].map(x => x + 1);

// Part 1
while (elves.length > 1) {
    const odd = elves.length % 2 === 1;
    elves = elves.filter((_, i) => i % 2 === 0);
    if (odd) {
        elves.unshift(elves.pop());
    }
}
console.log(elves);

elves = [...Array(numElves).keys()].map(x => x + 1);

// Part 2
while (elves.length > 1) {
    let nextStealer = 0;
    let nextStolenIndex = nextStealer + Math.floor((elves.length - nextStealer) / 2);

    // Filter out all indices that would get stolen before looping around
    elves = elves.filter((_, i) => {
        if (i === nextStolenIndex) {
            nextStealer++;

            // nextStealer * 2 to account for start pos + stolen elves to skip
            nextStolenIndex = (nextStealer * 2) + Math.floor((elves.length - nextStealer) / 2);
            return false;
        }
        return true;
    })

    elves = [...elves.slice(nextStealer), ...elves.slice(0, nextStealer)]
}
console.log(elves);