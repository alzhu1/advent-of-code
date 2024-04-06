use std::collections::{HashSet, VecDeque};

use aoc2022::read_file;

fn get_elevation(e: u8) -> u8 {
    match e {
        b'S' => b'a',
        b'E' => b'z',
        _ => e,
    }
}

fn get_point(grid: &[Vec<u8>], target: u8) -> (i16, i16) {
    grid.iter()
        .enumerate()
        .flat_map(|(r, row)| {
            row.iter()
                .enumerate()
                .flat_map(|(c, &col)| {
                    if col == target {
                        Some((r as i16, c as i16))
                    } else {
                        None
                    }
                })
                .collect::<Vec<(i16, i16)>>()
        })
        .collect::<Vec<(i16, i16)>>()
        .remove(0)
}

fn search(grid: &[Vec<u8>], start_pos: (i16, i16), target: u8, reversed: bool) -> u32 {
    let deltas = [(1, 0), (0, 1), (-1, 0), (0, -1)];
    let num_rows = grid.len() as i16;
    let num_cols = grid[0].len() as i16;

    let mut queue: VecDeque<((i16, i16), u32)> = VecDeque::new();
    queue.push_back((start_pos, 0));

    let mut cache: HashSet<(i16, i16)> = HashSet::new();

    while let Some((pos, dist)) = queue.pop_front() {
        if cache.contains(&pos) {
            continue;
        }
        cache.insert(pos);

        let curr_char = grid[pos.0 as usize][pos.1 as usize];
        if curr_char == target {
            return dist;
        }

        let next_items = deltas
            .iter()
            .map(|(x, y)| ((pos.0 + x, pos.1 + y), dist + 1))
            .filter(|((r, c), _)| (0..num_rows).contains(r) && (0..num_cols).contains(c))
            .filter(|((r, c), _)| {
                let curr_char = get_elevation(curr_char);
                let next_char = get_elevation(grid[*r as usize][*c as usize]);

                if reversed {
                    curr_char <= next_char + 1
                } else {
                    next_char <= curr_char + 1
                }
            });
        queue.extend(next_items);
    }

    u32::MAX
}

fn main() {
    let lines = read_file(file!(), "input.txt");

    let grid: Vec<Vec<u8>> = lines
        .map_while(Result::ok)
        .map(|line| line.as_bytes().to_vec())
        .collect();

    let start = get_point(&grid, b'S');
    let end = get_point(&grid, b'E');

    let p1 = search(&grid, start, b'E', false);
    let p2 = search(&grid, end, b'a', true);

    println!("Part 1: {}", p1);
    println!("Part 2: {}", p2);
}
