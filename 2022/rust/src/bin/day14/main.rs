use std::collections::HashSet;

use aoc2022::read_file;

fn spread(p: (i32, i32), cache: &mut HashSet<(i32, i32)>, floor_y: i32) {
    if cache.contains(&p) || p.1 >= floor_y {
        return;
    }

    cache.insert(p);

    spread((p.0 - 1, p.1 + 1), cache, floor_y);
    spread((p.0, p.1 + 1), cache, floor_y);
    spread((p.0 + 1, p.1 + 1), cache, floor_y);
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut cache: HashSet<(i32, i32)> = HashSet::new();

    for line in lines.map_while(Result::ok) {
        let points: Vec<(i32, i32)> = line
            .split(" -> ")
            .map(|x| {
                let (a, b) = x.split_once(',').unwrap();

                (a.parse().unwrap(), b.parse().unwrap())
            })
            .collect();

        cache.extend(
            points
                .iter()
                .zip(points.iter().skip(1))
                .flat_map(|(p1, p2)| {
                    // Get a list of points from p1 to p2
                    (p1.0.min(p2.0)..=p1.0.max(p2.0))
                        .flat_map(|x| {
                            (p1.1.min(p2.1)..=p1.1.max(p2.1))
                                .map(|y| (x, y))
                                .collect::<Vec<(i32, i32)>>()
                        })
                        .collect::<Vec<(i32, i32)>>()
                }),
        );
    }

    let floor_y = cache.iter().max_by(|x, y| x.1.cmp(&y.1)).unwrap().1 + 2;
    let rock_count = cache.len();

    loop {
        let mut p = (500, 0);

        while p.1 < floor_y - 1 {
            let down = (p.0, p.1 + 1);

            if !cache.contains(&down) {
                p = down;
                continue;
            }

            let down_left = (p.0 - 1, p.1 + 1);
            if !cache.contains(&down_left) {
                p = down_left;
                continue;
            }

            let down_right = (p.0 + 1, p.1 + 1);
            if !cache.contains(&down_right) {
                p = down_right;
                continue;
            }

            // Stable
            cache.insert(p);
            break;
        }

        if p.1 == floor_y - 1 {
            println!("Part 1: {}", cache.len() - rock_count);
            break;
        }
    }

    spread((500, 0), &mut cache, floor_y);

    println!("Part 2: {}", cache.len() - rock_count);
}
