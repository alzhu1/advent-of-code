use std::collections::HashSet;

use aoc2022::read_file;

fn is_close(head: &[i32; 2], tail: &[i32; 2]) -> bool {
    (head[0] - tail[0]).abs() <= 1 && (head[1] - tail[1]).abs() <= 1
}

fn move_rope(rope: &mut [[i32; 2]], set: &mut HashSet<[i32; 2]>, set2: &mut HashSet<[i32; 2]>) {
    for r in 0..rope.len() - 1 {
        if !is_close(&rope[r], &rope[r + 1]) {
            let (x_diff, y_diff) = (
                (rope[r][0] - rope[r + 1][0]).signum(),
                (rope[r][1] - rope[r + 1][1]).signum(),
            );

            rope[r + 1][0] += x_diff;
            rope[r + 1][1] += y_diff;

            if r == 0 {
                set.insert(rope[r + 1]);
            }

            if r == rope.len() - 2 {
                set2.insert(rope[r + 1]);
            }
        }
    }
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut set: HashSet<[i32; 2]> = HashSet::new();
    let mut set2: HashSet<[i32; 2]> = HashSet::new();
    set.insert([0, 0]);
    set2.insert([0, 0]);

    let mut rope: Vec<[i32; 2]> = vec![[0, 0]; 10];

    for line in lines.map_while(Result::ok) {
        let (direction, second) = line.split_at(1);

        let size = second.trim().parse::<i32>().unwrap();

        let mut delta = match direction {
            "U" => [0, size],
            "R" => [size, 0],
            "D" => [0, -size],
            "L" => [-size, 0],
            _ => unreachable!(),
        };

        let (x_delta, y_delta) = (delta[0].signum(), delta[1].signum());

        while delta[0] != 0 {
            rope[0][0] += x_delta;
            delta[0] -= x_delta;

            move_rope(&mut rope, &mut set, &mut set2);
        }

        while delta[1] != 0 {
            rope[0][1] += y_delta;
            delta[1] -= y_delta;

            move_rope(&mut rope, &mut set, &mut set2);
        }
    }

    println!("Part 1: {}", set.len());
    println!("Part 2: {}", set2.len());
}
