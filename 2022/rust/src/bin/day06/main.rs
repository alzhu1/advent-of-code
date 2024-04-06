use aoc2022::read_file;
use std::collections::{HashMap, HashSet};

fn get_marker(line: &str, marker_size: usize) -> usize {
    for i in marker_size..line.len() + marker_size {
        let char_set: HashSet<char> = line[i - marker_size..i].chars().collect();
        if char_set.len() == marker_size {
            return i;
        }
    }

    unreachable!();
}

#[allow(dead_code)]
fn get_marker2(line: &str, marker_size: usize) -> usize {
    let mut char_map: HashMap<char, usize> =
        line[0..marker_size]
            .chars()
            .fold(HashMap::new(), |mut map, c| {
                *map.entry(c).or_insert(0) += 1;
                map
            });

    let chars: Vec<char> = line.chars().collect();

    for i in 0..line.len() - marker_size {
        if char_map.len() == marker_size {
            return i + marker_size;
        }

        let c1 = chars[i];
        let c2 = chars[i + marker_size];

        *char_map.get_mut(&c1).unwrap() -= 1;
        if char_map[&c1] == 0 {
            char_map.remove(&c1);
        }

        if let Some(entry) = char_map.get_mut(&c2) {
            *entry += 1;
        } else {
            char_map.insert(c2, 1);
        }
    }

    unreachable!();
}

fn main() {
    let lines = read_file(file!(), "input.txt");

    for line in lines.map_while(Result::ok) {
        println!("Part 1: {}", get_marker(&line, 4));
        println!("Part 2: {}", get_marker(&line, 14));
    }
}
