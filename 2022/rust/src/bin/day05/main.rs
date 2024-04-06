use aoc2022::read_file;
use std::collections::VecDeque;

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut stack = vec![
        // VecDeque::<char>::from(vec!('N', 'Z')),
        // VecDeque::<char>::from(vec!('D', 'C', 'M')),
        // VecDeque::<char>::from(vec!('P')),
        VecDeque::<char>::from(vec!['G', 'B', 'D', 'C', 'P', 'R']),
        VecDeque::<char>::from(vec!['G', 'V', 'H']),
        VecDeque::<char>::from(vec!['M', 'P', 'J', 'D', 'Q', 'S', 'N']),
        VecDeque::<char>::from(vec!['M', 'N', 'C', 'D', 'G', 'L', 'S', 'P']),
        VecDeque::<char>::from(vec!['S', 'L', 'F', 'P', 'C', 'N', 'B', 'J']),
        VecDeque::<char>::from(vec!['S', 'T', 'G', 'V', 'Z', 'D', 'B', 'Q']),
        VecDeque::<char>::from(vec!['Q', 'T', 'F', 'H', 'M', 'Z', 'B']),
        VecDeque::<char>::from(vec!['F', 'B', 'D', 'M', 'C']),
        VecDeque::<char>::from(vec!['G', 'Q', 'C', 'F']),
    ];
    let mut stack2 = stack.clone();

    for line in lines.map_while(Result::ok) {
        let nums: Vec<usize> = line
            .split_whitespace()
            .map(|x| x.parse::<usize>())
            .filter(Result::is_ok)
            .map(|x| x.unwrap())
            .collect();

        for i in 1..=nums[0] {
            // Part 1
            if let Some(c) = stack[nums[1] - 1].pop_front() {
                stack[nums[2] - 1].push_front(c);
            }

            // Part 2
            if let Some(c) = stack2[nums[1] - 1].pop_front() {
                stack2[nums[2] - 1].insert(i - 1, c);
            }
        }
    }

    let s: String = stack.iter().map(|x| x.front().unwrap()).collect();
    let s2: String = stack2.iter().map(|x| x.front().unwrap()).collect();

    println!("{} {}", s, s2);
}
