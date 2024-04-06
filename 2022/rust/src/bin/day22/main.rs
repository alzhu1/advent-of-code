use std::{cmp::max, collections::HashSet};

use aoc2022::read_file;

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut line_iter = lines.map_while(Result::ok);

    // (row, col)
    let mut grid: Vec<Vec<u8>> = vec![];
    let mut open: HashSet<(i32, i32)> = HashSet::new();
    let mut walls: HashSet<(i32, i32)> = HashSet::new();

    let mut dimensions: (i32, i32) = (0, 0);

    for line in line_iter.by_ref() {
        if line.is_empty() {
            break;
        }

        dimensions.0 += 1;
        dimensions.1 = max(dimensions.1, line.len() as i32);

        grid.push(line.as_bytes().to_vec());
        let row_pos = (grid.len() - 1) as i32;

        for (i, c) in line.as_bytes().iter().enumerate() {
            match *c {
                b'.' => open.insert((row_pos, i as i32)),
                b'#' => walls.insert((row_pos, i as i32)),
                _ => false,
            };
        }
    }

    println!("Dimensions: {:?}", dimensions);

    let surface_area: i32 = grid
        .iter()
        .map(|v| {
            v.iter()
                .map(|&c| if c == b' ' { 0 } else { 1 })
                .sum::<i32>()
        })
        .sum();
    let face_size = ((surface_area / 6) as f64).sqrt() as i32;
    println!("{}", face_size);

    let face_corners: Vec<(i32, i32)> = grid
        .iter()
        .enumerate()
        .flat_map(|(r, v)| {
            v.iter()
                .enumerate()
                .map(|(c, cc)| {
                    let row = r as i32;
                    let col = c as i32;
                    if row % face_size == 0 && col % face_size == 0 && *cc != b' ' {
                        (row, col)
                    } else {
                        (-1, -1)
                    }
                })
                .collect::<Vec<(i32, i32)>>()
        })
        .filter(|p| p.0 >= 0 && p.1 >= 0)
        .collect();

    // Creating a general solution is too annoying, this vec is hardcoded for the inputs
    // let next_faces: Vec<[usize; 4]> = vec![
    //     [5, 3, 2, 1],
    //     [2, 4, 5, 0],
    //     [3, 4, 1, 0],
    //     [5, 4, 2, 0],
    //     [5, 1, 2, 3],
    //     [0, 1, 4, 3],
    // ];
    let next_faces: Vec<[usize; 4]> = vec![
        [1, 2, 3, 5],
        [4, 2, 0, 5],
        [1, 4, 3, 0],
        [4, 5, 0, 2],
        [1, 5, 3, 2],
        [4, 1, 0, 3],
    ];

    let deltas = [[0, 1], [1, 0], [0, -1], [-1, 0]];

    let input = line_iter.next().unwrap();
    let mut curr_pos = (0, 0);
    while grid[0][curr_pos.1 as usize] == b' ' {
        curr_pos.1 += 1;
    }

    let mut curr_pos_2 = curr_pos;

    let mut input_iter = input.as_bytes().iter().peekable();

    // 0 is right, clockwise
    let mut curr_dir: i32 = 0;
    let mut curr_dir_2: i32 = 0;

    while let Some(c) = input_iter.next() {
        match *c {
            b'R' => {
                curr_dir = (curr_dir + 1) % 4;
                curr_dir_2 = (curr_dir_2 + 1) % 4;
                continue;
            }
            b'L' => {
                curr_dir = (curr_dir - 1).rem_euclid(4);
                curr_dir_2 = (curr_dir_2 - 1).rem_euclid(4);
                continue;
            }
            _ => (),
        };

        let mut num = vec![*c];

        while let Some(peek) = input_iter.peek() {
            let p = **peek;
            if p == b'R' || p == b'L' {
                break;
            }

            if let Some(cc) = input_iter.next() {
                num.push(*cc);
            } else {
                break;
            }
        }

        let amount = String::from_utf8(num).unwrap().parse::<i32>().unwrap();
        let dir_delta = deltas[curr_dir as usize];
        let mut dir_delta_2 = deltas[curr_dir_2 as usize];

        for _ in 0..amount {
            let mut check_pos = (curr_pos.0 + dir_delta[0], curr_pos.1 + dir_delta[1]);

            if walls.contains(&check_pos) {
                break;
            }

            if open.contains(&check_pos) {
                curr_pos = check_pos;
            } else {
                // Should be empty space, keep adding to check_pos until we get back to walls or open

                loop {
                    check_pos.0 = (check_pos.0 + dir_delta[0]).rem_euclid(dimensions.0);
                    check_pos.1 = (check_pos.1 + dir_delta[1]).rem_euclid(dimensions.1);

                    if walls.contains(&check_pos) || open.contains(&check_pos) {
                        break;
                    }

                    // println!("{:?}", check_pos);
                }

                if walls.contains(&check_pos) {
                    break;
                }
                curr_pos = check_pos;
            }
        }

        for _ in 0..amount {
            println!("BEGIN: {:?}", curr_pos_2);
            let mut check_pos_2 = (curr_pos_2.0 + dir_delta_2[0], curr_pos_2.1 + dir_delta_2[1]);

            if walls.contains(&check_pos_2) {
                break;
            }

            if open.contains(&check_pos_2) {
                curr_pos_2 = check_pos_2;
            } else {
                // TODO: Landed in empty space, need to check where we are going to go
                let curr_face_index = face_corners
                    .iter()
                    .enumerate()
                    .map(|(i, c)| {
                        let row_check = c.0..c.0 + face_size;
                        let col_check = c.1..c.1 + face_size;

                        if row_check.contains(&curr_pos_2.0) && col_check.contains(&curr_pos_2.1) {
                            i
                        } else {
                            0
                        }
                    })
                    .max()
                    .unwrap();

                let curr_face_corner = face_corners[curr_face_index];

                let next_face_index = next_faces[curr_face_index][curr_dir_2 as usize];
                let next_face_corner = face_corners[next_face_index];
                let enter_on_next_face = next_faces[next_face_index]
                    .iter()
                    .enumerate()
                    .filter(|(_, c)| curr_face_index == **c)
                    .map(|(i, _)| i)
                    .max()
                    .unwrap();

                // TODO: At this point, I need to know how far I am from curr face
                println!(
                    "Curr corner = {}, next one should be {} (moving {})",
                    curr_face_index, next_face_index, curr_dir_2
                );
                println!("Entrance on next face is {}", enter_on_next_face);

                println!("Curr face corner: {:?}", curr_face_corner);
                println!("DEBUG {}", curr_face_corner.1 + face_size - 1);

                let offset = match curr_dir_2 {
                    0 => curr_pos_2.0 - curr_face_corner.0,
                    1 => (curr_face_corner.1 + face_size - 1) - curr_pos_2.1,
                    2 => (curr_face_corner.0 + face_size - 1) - curr_pos_2.0,
                    3 => curr_pos_2.1 - curr_face_corner.1,
                    _ => unreachable!(),
                };

                println!("Curr pos = {:?}, offset is {}", curr_pos_2, offset);
                println!("Next face corner: {:?}", next_face_corner);

                check_pos_2 = match enter_on_next_face {
                    0 => (
                        next_face_corner.0 + face_size - offset - 1,
                        next_face_corner.1 + face_size - 1,
                    ),
                    1 => (
                        next_face_corner.0 + face_size - 1,
                        next_face_corner.1 + offset,
                    ),
                    2 => (next_face_corner.0 + offset, next_face_corner.1),
                    3 => (
                        next_face_corner.0,
                        next_face_corner.1 + face_size - offset - 1,
                    ),
                    _ => unreachable!(),
                };

                println!("Next check pos is at {:?}", check_pos_2);

                if walls.contains(&check_pos_2) {
                    break;
                }
                curr_pos_2 = check_pos_2;
                curr_dir_2 = (enter_on_next_face as i32 + 2) % 4;

                dir_delta_2 = deltas[curr_dir_2 as usize];

                println!("Final pos and dir: {:?} and {}\n", curr_pos_2, curr_dir_2);
            }
        }
    }

    println!("Final: {:?}", curr_pos);
    let password = (1000 * (curr_pos.0 + 1)) + (4 * (curr_pos.1 + 1)) + curr_dir;
    let password_2 = (1000 * (curr_pos_2.0 + 1)) + (4 * (curr_pos_2.1 + 1)) + curr_dir_2;
    println!("Password: {}", password);
    println!("Password 2: {}", password_2);
}
