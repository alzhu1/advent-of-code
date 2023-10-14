const crypto = require('crypto');

let salt = 'ngcjuoqr';

let keys = [];
let candidates = [];

let i = 0;
while (keys.length < 80) {
    if (candidates.length > 0 && candidates[0][0] < i - 1000) {
        candidates.shift();
    }

    const checkKey = `${salt}${i}`;
    let hash = crypto.createHash('md5').update(checkKey).digest('hex');

    // Part 2
    for (let h = 0; h < 2016; h++) {
        hash = crypto.createHash('md5').update(hash).digest('hex');
    }

    for (let c = 0; c < hash.length - 2; c++) {
        const sub = hash.substring(c, c+ 3);

        // Found first triplet, mark as candidate
        if (sub[0] === sub[1] && sub[0] === sub[2]) {
            candidates.push([i, sub[0]]);
            break;
        }
    }

    for (let c = 0; c < hash.length - 4; c++) {
        const sub = hash.substring(c, c+ 5);

        let quintuplet = true;
        for (let cc = 0; cc < sub.length - 1; cc++) {
            if (sub[cc] !== sub[cc + 1]) {
                quintuplet = false;
                break;
            }
        }

        if (quintuplet) {
            // Loop through candidates to see if it matches character
            let cand = 0;
            while (cand < candidates.length) {
                const [candidateIndex, candidateChar] = candidates[cand];

                // Ignore same index values
                if (candidateIndex !== i && candidateChar === sub[0]) {
                    console.log('key index found at ', candidateIndex, i);
                    keys.push(candidateIndex);
                    candidates.splice(cand, 1);
                    cand--;
                }
                cand++;
            }
        }
    }

    i++;
}

keys.sort((a, b) => a - b);
console.log(keys);
console.log(keys[63]);