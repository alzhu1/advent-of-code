use std::{
    cmp::max,
    collections::{HashSet, VecDeque},
};

use aoc2022::read_file;

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut points: HashSet<(i16, i16, i16)> = HashSet::new();

    let mut max_dimensions = (0, 0, 0);

    for line in lines.map_while(Result::ok) {
        let point_vec: Vec<i16> = line.split(',').map(|x| x.parse::<i16>().unwrap()).collect();
        let point = (point_vec[0], point_vec[1], point_vec[2]);

        points.insert(point);

        max_dimensions.0 = max(max_dimensions.0, point.0);
        max_dimensions.1 = max(max_dimensions.1, point.1);
        max_dimensions.2 = max(max_dimensions.2, point.2);
    }

    println!("Max dimensions: {:?}", max_dimensions);

    let deltas = [
        (1, 0, 0),
        (-1, 0, 0),
        (0, 1, 0),
        (0, -1, 0),
        (0, 0, 1),
        (0, 0, -1),
    ];
    let mut count: usize = points
        .iter()
        .map(|p| {
            deltas
                .iter()
                .map(|(dx, dy, dz)| (p.0 + dx, p.1 + dy, p.2 + dz))
                .filter(|pp| !points.contains(pp))
                .count()
        })
        .sum();

    println!("Part 1: {}", count);

    let x_bounds = 0..=max_dimensions.0;
    let y_bounds = 0..=max_dimensions.1;
    let z_bounds = 0..=max_dimensions.2;

    let mut cache: HashSet<(i16, i16, i16)> = HashSet::new();
    let mut queue: VecDeque<(i16, i16, i16)> = VecDeque::new();
    queue.push_back(max_dimensions);
    while let Some(p) = queue.pop_front() {
        let (x, y, z) = p;
        if x_bounds.contains(&x) && y_bounds.contains(&y) && z_bounds.contains(&z) {
            if points.contains(&p) || cache.contains(&p) {
                continue;
            }
            cache.insert(p);

            for (dx, dy, dz) in deltas {
                let neighbor = (x + dx, y + dy, z + dz);
                queue.push_back(neighbor);
            }
        }
    }

    let possibles: Vec<(i16, i16, i16)> = x_bounds
        .clone()
        .flat_map(|x| {
            y_bounds
                .clone()
                .flat_map(|y| {
                    z_bounds
                        .clone()
                        .map(|z| (x, y, z))
                        .collect::<Vec<(i16, i16, i16)>>()
                })
                .collect::<Vec<(i16, i16, i16)>>()
        })
        .collect();

    for p in possibles {
        if points.contains(&p) || cache.contains(&p) {
            continue;
        }

        // We are inside of a block, let's check it out
        queue.clear();
        queue.push_back(p);

        while let Some(check_p) = queue.pop_front() {
            let (check_x, check_y, check_z) = check_p;
            if x_bounds.contains(&check_x)
                && y_bounds.contains(&check_y)
                && z_bounds.contains(&check_z)
            {
                if points.contains(&p) || cache.contains(&p) {
                    continue;
                }
                cache.insert(p);

                for (dx, dy, dz) in deltas {
                    let neighbor = (check_x + dx, check_y + dy, check_z + dz);
                    queue.push_back(neighbor);

                    if points.contains(&neighbor) {
                        count -= 1;
                    }
                }
            }
        }
    }

    println!("Part 2: {}", count);
}
