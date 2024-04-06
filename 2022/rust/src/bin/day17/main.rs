use std::{
    cmp::{max, min},
    collections::{HashMap, HashSet},
};

use aoc2022::read_file;

#[derive(Debug, Clone, Copy, Hash, PartialEq, Eq)]
enum BlockType {
    Horizontal,
    Plus,
    L,
    Vertical,
    Square,
}

impl BlockType {
    fn get_type(block_count: usize) -> Self {
        match block_count % 5 {
            0 => BlockType::Horizontal,
            1 => BlockType::Plus,
            2 => BlockType::L,
            3 => BlockType::Vertical,
            4 => BlockType::Square,
            _ => unreachable!(),
        }
    }
}

#[derive(Debug)]
struct Block {
    bottom_left: (usize, usize),
    block_type: BlockType,
}

impl Block {
    fn new(bottom_y: usize, block_count: usize) -> Self {
        Block {
            bottom_left: (2, bottom_y + 4),
            block_type: BlockType::get_type(block_count),
        }
    }

    fn get_bounds(&self) -> (usize, usize) {
        match self.block_type {
            BlockType::Horizontal => (4, 1),
            BlockType::Plus => (3, 3),
            BlockType::L => (3, 3),
            BlockType::Vertical => (1, 4),
            BlockType::Square => (2, 2),
        }
    }

    fn get_side_move_checks(&self, right: bool) -> Vec<(usize, usize)> {
        let (x, y) = self.bottom_left;
        let check_x = if !right {
            x - 1
        } else {
            x + self.get_bounds().0
        };
        let inner_x = if !right { check_x + 1 } else { check_x - 1 };

        match self.block_type {
            BlockType::Horizontal => vec![(check_x, y)],
            BlockType::Plus => vec![(inner_x, y), (check_x, y + 1), (inner_x, y + 2)],
            BlockType::L => {
                // If right, return 3 of check_x
                if right {
                    vec![(check_x, y), (check_x, y + 1), (check_x, y + 2)]
                } else {
                    // Otherwise, check_x for bottom. check_x + 2 for the other 2
                    vec![(check_x, y), (check_x + 2, y + 1), (check_x + 2, y + 2)]
                }
            }
            BlockType::Vertical => vec![
                (check_x, y),
                (check_x, y + 1),
                (check_x, y + 2),
                (check_x, y + 3),
            ],
            BlockType::Square => vec![(check_x, y), (check_x, y + 1)],
        }
    }

    fn get_down_move_checks(&self) -> Vec<(usize, usize)> {
        let (x, y) = self.bottom_left;
        let down_y = y - 1;
        match self.block_type {
            BlockType::Horizontal => vec![
                (x, down_y),
                (x + 1, down_y),
                (x + 2, down_y),
                (x + 3, down_y),
            ],
            BlockType::Plus => vec![(x, down_y + 1), (x + 1, down_y), (x + 2, down_y + 1)],
            BlockType::L => vec![(x, down_y), (x + 1, down_y), (x + 2, down_y)],
            BlockType::Vertical => vec![(x, down_y)],
            BlockType::Square => vec![(x, down_y), (x + 1, down_y)],
        }
    }

    fn get_blocks(&self) -> Vec<(usize, usize)> {
        let (x, y) = self.bottom_left;
        match self.block_type {
            BlockType::Horizontal => vec![(x, y), (x + 1, y), (x + 2, y), (x + 3, y)],
            BlockType::Plus => vec![
                (x, y + 1),
                (x + 1, y),
                (x + 1, y + 1),
                (x + 1, y + 2),
                (x + 2, y + 1),
            ],
            BlockType::L => vec![
                (x, y),
                (x + 1, y),
                (x + 2, y),
                (x + 2, y + 1),
                (x + 2, y + 2),
            ],
            BlockType::Vertical => vec![(x, y), (x, y + 1), (x, y + 2), (x, y + 3)],
            BlockType::Square => vec![(x, y), (x + 1, y), (x, y + 1), (x + 1, y + 1)],
        }
    }

    fn simulate(
        &mut self,
        bytes: &[u8],
        index: &mut usize,
        blocks: &mut HashSet<(usize, usize)>,
    ) -> usize {
        for _ in 0..3 {
            let c = bytes[*index];
            *index = (*index + 1) % bytes.len();
            match c {
                b'>' => self.bottom_left.0 = min(self.bottom_left.0 + 1, 7 - self.get_bounds().0),
                b'<' => self.bottom_left.0 = self.bottom_left.0.saturating_sub(1),
                _ => unreachable!(),
            }
            self.bottom_left.1 -= 1;
        }

        let top_y = self.bottom_left.1 - 1;
        loop {
            let c = bytes[*index];
            *index = (*index + 1) % bytes.len();

            let right = c == b'>';
            let can_move_left = !right && self.bottom_left.0 > 0;
            let can_move_right = right && self.bottom_left.0 + self.get_bounds().0 < 7;

            if (can_move_left || can_move_right)
                && self
                    .get_side_move_checks(right)
                    .iter()
                    .all(|c| !blocks.contains(c))
            {
                self.bottom_left.0 = if right {
                    self.bottom_left.0 + 1
                } else {
                    self.bottom_left.0 - 1
                };
            }

            if self
                .get_down_move_checks()
                .iter()
                .all(|c| !blocks.contains(c))
            {
                self.bottom_left.1 -= 1;
            } else {
                break;
            }
        }

        blocks.extend(self.get_blocks());
        max(top_y, self.bottom_left.1 + self.get_bounds().1 - 1)
    }
}

fn main() {
    let line = read_file(file!(), "input.txt").next().unwrap().unwrap();
    let bytes = line.as_bytes();

    let mut blocks: HashSet<(usize, usize)> = (0..=7).map(|x| (x, 0)).collect();

    let mut top_y = 0;
    let mut index = 0;

    let mut wind_map: HashMap<(usize, BlockType), (usize, usize, usize)> = HashMap::new();

    let mut part_complete = [false, false];

    for b in 0..1000000000000 {
        let last_wind_index = index;
        top_y = Block::new(top_y, b).simulate(bytes, &mut index, &mut blocks);

        if part_complete[0] && part_complete[1] {
            break;
        }

        if b == 2021 {
            println!("Part 1: {}", top_y);
            part_complete[0] = true;
        }

        let block_type = BlockType::get_type(b);
        let wind_key = (last_wind_index, block_type);

        if !part_complete[1] && wind_map.contains_key(&wind_key) {
            let (stored_w, first_top_y, first_b) = wind_map[&wind_key];

            if stored_w == index {
                // Only check if this is part of the loop
                let loop_size = b - first_b;
                let height_gain = top_y - first_top_y;

                let num_loops = (1000000000000 - b - 1) / loop_size;
                let mut simulate_top_y = top_y + num_loops * height_gain;

                let next_b = b + num_loops * loop_size;

                let mut curr_key = (last_wind_index, block_type);

                for _ in next_b + 1..1000000000000 {
                    let (w, curr_top_y, curr_b) = wind_map[&curr_key];

                    let next_block_type = BlockType::get_type(curr_b + 1);
                    let (_, next_top_y, _) = wind_map[&(w, next_block_type)];

                    simulate_top_y += next_top_y - curr_top_y;

                    curr_key = (w, next_block_type);
                }

                println!("Part 2: {}", simulate_top_y);
                part_complete[1] = true;
            }
        }
        wind_map.insert(wind_key, (index, top_y, b));
    }
}
