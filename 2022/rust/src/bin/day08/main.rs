use std::cmp::max;

use aoc2022::read_file;

fn check_grid(grid: &mut [Vec<[u32; 2]>]) {
    let len = grid.len();

    // Order: right, left, down, up
    let ranges = [
        (0..=len - 1).collect::<Vec<usize>>(),
        (0..=len - 1).rev().collect::<Vec<usize>>(),
    ];

    for range in ranges.iter() {
        #[allow(clippy::needless_range_loop)]
        for r in 0..len {
            let mut max_height = 0;
            for c in range.clone() {
                if grid[r][c][1] == 1 {
                    max_height = max(max_height, grid[r][c][0]);
                    continue;
                }

                if grid[r][c][0] > max_height {
                    grid[r][c][1] = 1;
                }

                max_height = max(max_height, grid[r][c][0]);
            }
        }

        for c in 0..len {
            let mut max_height = 0;
            for r in range.clone() {
                if grid[r][c][1] == 1 {
                    max_height = max(max_height, grid[r][c][0]);
                    continue;
                }

                if grid[r][c][0] > max_height {
                    grid[r][c][1] = 1;
                }

                max_height = max(max_height, grid[r][c][0]);
            }
        }
    }
}

fn get_scenic_score(grid: &[Vec<[u32; 2]>], r: usize, c: usize) -> usize {
    let len = grid.len();
    let mut scenic_score = 1;

    for up in (0..r).rev() {
        if up == 0 {
            scenic_score *= r - up;
        } else if grid[up][c][0] >= grid[r][c][0] {
            scenic_score *= r - up;
            break;
        }
    }

    for right in c + 1..len {
        if right == len - 1 {
            scenic_score *= right - c;
        } else if grid[r][right][0] >= grid[r][c][0] {
            scenic_score *= right - c;
            break;
        }
    }

    for down in r + 1..len {
        if down == len - 1 {
            scenic_score *= down - r;
        } else if grid[down][c][0] >= grid[r][c][0] {
            scenic_score *= down - r;
            break;
        }
    }

    for left in (0..c).rev() {
        if left == 0 {
            scenic_score *= c - left;
        } else if grid[r][left][0] >= grid[r][c][0] {
            scenic_score *= c - left;
            break;
        }
    }

    scenic_score
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut grid: Vec<Vec<[u32; 2]>> = lines
        .map_while(Result::ok)
        .enumerate()
        .map(|(i, line)| {
            let len = line.len();
            line.chars()
                .enumerate()
                .map(|(j, x)| {
                    [
                        x.to_digit(10).unwrap(),
                        if i == 0 || i == len - 1 || j == 0 || j == len - 1 {
                            1
                        } else {
                            0
                        },
                    ]
                })
                .collect::<Vec<[u32; 2]>>()
        })
        .collect();

    check_grid(&mut grid);

    let final_visible: u32 = grid
        .iter()
        .map(|row| row.iter().map(|x| x[1]).sum::<u32>())
        .sum();

    let mut max_scenic_score = 0;
    for r in 1..grid.len() - 1 {
        for c in 1..grid.len() - 1 {
            max_scenic_score = max(max_scenic_score, get_scenic_score(&grid, r, c));
        }
    }

    println!("Part 1: {}", final_visible);
    println!("Part 2: {}", max_scenic_score);
}
