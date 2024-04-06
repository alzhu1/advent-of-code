use std::collections::{HashMap, HashSet, VecDeque};

use aoc2022::read_file;

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut elves: HashSet<(i32, i32)> = HashSet::new();

    for (y, line) in lines.map_while(Result::ok).enumerate() {
        for (x, c) in line.as_bytes().iter().enumerate() {
            if *c == b'#' {
                elves.insert((x as i32, y as i32));
            }
        }
    }

    let checks: Vec<[(i32, i32); 2]> = vec![
        [(-1, -1), (0, -1)],
        [(1, -1), (1, 0)],
        [(1, 1), (0, 1)],
        [(-1, 1), (-1, 0)],
    ];
    let mut order: VecDeque<usize> = vec![0, 2, 3, 1].into_iter().collect();

    // Hash map from final location to the elves going there
    let mut elf_moves: HashMap<(i32, i32), HashSet<(i32, i32)>> = HashMap::new();

    let mut iters = 0;
    loop {
        for elf in elves.iter() {
            let neighbors = checks
                .iter()
                .flatten()
                .filter(|(dx, dy)| elves.contains(&(elf.0 + dx, elf.1 + dy)))
                .count();

            if neighbors > 0 {
                for &c in order.iter() {
                    let curr_check = [
                        checks[c][0],
                        checks[c][1],
                        checks[(c + 1) % checks.len()][0],
                    ];

                    if curr_check
                        .iter()
                        .all(|(dx, dy)| !elves.contains(&(elf.0 + dx, elf.1 + dy)))
                    {
                        let (dx, dy) = checks[c][1];
                        let elf_move = (elf.0 + dx, elf.1 + dy);
                        (*elf_moves.entry(elf_move).or_default()).insert(*elf);

                        break;
                    }
                }
            }
        }

        let mut done = true;
        for (elf_move, elves_to_move) in elf_moves.drain() {
            if elves_to_move.len() == 1 {
                for elf in elves_to_move {
                    elves.remove(&elf);
                    elves.insert(elf_move);
                }

                done = false;
            }
        }

        iters += 1;
        if done {
            println!("Part 2: {}", iters);
            break;
        }

        if let Some(o) = order.pop_front() {
            order.push_back(o);
        }

        if iters == 10 {
            let min_x = elves.iter().map(|e| e.0).min().unwrap();
            let max_x = elves.iter().map(|e| e.0).max().unwrap();

            let min_y = elves.iter().map(|e| e.1).min().unwrap();
            let max_y = elves.iter().map(|e| e.1).max().unwrap();

            let x_dim = (max_x - min_x).abs() + 1;
            let y_dim = (max_y - min_y).abs() + 1;

            println!("Part 1: {}", x_dim * y_dim - elves.len() as i32);
        }
    }
}
