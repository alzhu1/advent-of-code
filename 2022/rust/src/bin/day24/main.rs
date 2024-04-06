use std::collections::{HashSet, VecDeque};

use aoc2022::read_file;

type BlizzardSet = HashSet<((i16, i16), (i16, i16))>;

fn gcd(a: i16, b: i16) -> i16 {
    if b == 0 {
        a
    } else {
        gcd(b, a % b)
    }
}

fn lcm(a: i16, b: i16) -> usize {
    (a * b / gcd(a, b)) as usize
}

fn get_next_blizzards(curr_blizzards: &BlizzardSet, height: i16, width: i16) -> BlizzardSet {
    let mut next_blizzards = HashSet::new();

    for (point, dir) in curr_blizzards {
        let mut next_point = (point.0 + dir.0, point.1 + dir.1);

        next_point.0 = match next_point.0 {
            0 => width - 2,
            w if w >= width - 1 => 1,
            _ => next_point.0
        };

        next_point.1 = match next_point.1 {
            0 => height - 2,
            h if h >= height - 1 => 1,
            _ => next_point.1
        };

        next_blizzards.insert((next_point, *dir));
    }

    next_blizzards
}

fn simulate(start: (i16, i16), end: (i16, i16), minute: usize, width: i16, height: i16, loop_point: usize, blizzards: &mut Vec<BlizzardSet>) -> usize {
    let mut cache: HashSet<((i16, i16), usize)> = HashSet::new();
    let mut queue: VecDeque<((i16, i16), usize)> = VecDeque::new();
    queue.push_back((start, minute));

    let deltas = [[1, 0], [0, 1], [-1, 0], [0, -1]];

    while let Some(((x, y), minute)) = queue.pop_front() {
        let loop_minute = minute % loop_point;
        let cache_check = ((x, y), loop_minute);

        if cache.contains(&cache_check) {
            continue;
        }
        cache.insert(cache_check);

        // Done
        if (x, y) == end {
            return minute;
        }

        // Hitting a border
        if (x, y) != start && (x <= 0 || y <= 0 || x >= width - 1 || y >= height - 1) {
            continue;
        }

        // Conflicts with curr blizzards
        let curr_blizzards = &blizzards[loop_minute];
        if deltas.iter().any(|[dx, dy]| curr_blizzards.contains(&((x, y), (*dx, *dy)))) {
            continue;
        }

        let next_grid_index = (loop_minute + 1) % loop_point;
        if blizzards.len() <= next_grid_index {
            let next_blizzards = get_next_blizzards(curr_blizzards, height, width);
            blizzards.push(next_blizzards);
        }

        for [dx, dy] in deltas.iter() {
            queue.push_back(((x + dx, y + dy), minute + 1));
        }
        queue.push_back(((x, y), minute + 1));
    }

    0
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut blizzards: Vec<BlizzardSet> = vec!(HashSet::new());

    let mut height: i16 = 0;
    let mut width: i16 = 0;

    for (y, line) in lines.map_while(Result::ok).enumerate() {
        for (x, l) in line.as_bytes().iter().enumerate() {
            let direction = match *l {
                b'>' => (1, 0),
                b'v' => (0, 1),
                b'<' => (-1, 0),
                b'^' => (0, -1),
                _ => (0, 0)
            };

            if direction.0 != 0 || direction.1 != 0 {
                blizzards[0].insert(((x as i16, y as i16), direction));
            }
        }

        width = line.len() as i16;
        height += 1;
    }

    let loop_point = lcm(width - 2, height - 2);

    let start = (1, 0);
    let end = (width - 2, height - 1);

    let to_end_minutes = simulate(start, end, 0, width, height, loop_point, &mut blizzards);
    println!("Part 1: {}", to_end_minutes);

    let back_to_start_minutes = simulate(end, start, to_end_minutes, width, height, loop_point, &mut blizzards);
    let back_to_end_minutes = simulate(start, end, back_to_start_minutes, width, height, loop_point, &mut blizzards);
    println!("Part 2: {}", back_to_end_minutes);
}
