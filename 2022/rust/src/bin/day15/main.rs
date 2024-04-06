use std::collections::HashSet;

use aoc2022::read_file;

fn get_point(s: &str) -> (i32, i32) {
    let p_rev = s
        .split_whitespace()
        .rev()
        .take(2)
        .map(|x| {
            x.split_at(2)
                .1
                .split(',')
                .take(1)
                .collect::<String>()
                .parse::<i32>()
                .unwrap()
        })
        .collect::<Vec<i32>>();

    (p_rev[1], p_rev[0])
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut set: HashSet<(i32, i32)> = HashSet::new();
    let mut beacons: HashSet<(i32, i32)> = HashSet::new();

    let mut possibles: HashSet<(i32, i32)> = HashSet::new();

    let mut sensors: Vec<((i32, i32), i32)> = vec![];

    let target = 2000000;
    let search_bounds = 0..=4000000;

    for line in lines.map_while(Result::ok) {
        let (first, second) = line.split_at(line.find(':').unwrap());

        let sensor = get_point(first);
        let beacon = get_point(second);
        let mut dist = (sensor.0 - beacon.0).abs() + (sensor.1 - beacon.1).abs();

        sensors.push((sensor, dist));

        let outer_dist = dist + 1;
        beacons.insert(beacon);

        let mut y_diff = 0;
        println!(
            "Processing sensor {:?} and beacon {:?} (dist {})",
            sensor, beacon, dist
        );
        for x in sensor.0 - outer_dist..=sensor.0 + outer_dist {
            if search_bounds.contains(&x) {
                if search_bounds.contains(&(sensor.1 - y_diff)) {
                    possibles.insert((x, sensor.1 - y_diff));
                }
                if search_bounds.contains(&(sensor.1 + y_diff)) {
                    possibles.insert((x, sensor.1 + y_diff));
                }
            }

            if x < sensor.0 {
                y_diff += 1;
            } else {
                y_diff -= 1;
            }
        }

        let dist_to_target = (sensor.1 - target).abs();
        if dist_to_target > dist {
            continue;
        }
        dist -= dist_to_target;

        set.extend((sensor.0 - dist..=sensor.0 + dist).map(|x| (x, target)));
    }

    let target_row_beacons = beacons.iter().filter(|b| b.1 == target).count();

    println!("Part 1: {:?}", set.len() - target_row_beacons);

    // Some idea from reddit: check the outside of a sensor's range
    // Since there's only 1 open spot, we can look at the squares on (sensor range + 1)
    // Because every other square must be occupied, therefore it sits right outside
    println!("Starting to check possibles (len {})", possibles.len());
    for possible in possibles {
        if sensors.iter().all(|(sensor, dist)| {
            let possible_dist = (sensor.0 - possible.0).abs() + (sensor.1 - possible.1).abs();

            possible_dist > *dist
        }) {
            println!("Point: {:?}", possible);

            let x: i64 = possible.0.into();
            let y: i64 = possible.1.into();
            println!("Product: {}", x * 4000000 + y);

            break;
        }
    }
}
