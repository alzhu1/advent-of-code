use std::ops::RangeInclusive;

use aoc2022::read_file;

fn get_overlap(bound: &RangeInclusive<i32>, check: &RangeInclusive<i32>) -> (bool, bool) {
    let (bound_start, bound_end) = (bound.start(), bound.end());
    let (check_start, check_end) = (check.start(), check.end());

    (
        // Part 1: bound completely contains check
        bound_start <= check_start && bound_end >= check_end,
        // Part 2: bound overlaps with check
        bound_start <= check_end && bound_end >= check_start,
    )
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut count = [0, 0];

    for line in lines.map_while(Result::ok) {
        let (first, second) = line.split_once(',').unwrap();

        let first_range = match first
            .split('-')
            .map(|x| x.parse::<i32>().unwrap())
            .collect::<Vec<i32>>()[..2]
        {
            [a, b] => a..=b,
            _ => unreachable!(),
        };

        let second_range = match second
            .split('-')
            .map(|x| x.parse::<i32>().unwrap())
            .collect::<Vec<i32>>()[..2]
        {
            [a, b] => a..=b,
            _ => unreachable!(),
        };

        let (first_contains, first_overlap) = get_overlap(&first_range, &second_range);
        let (second_contains, second_overlap) = get_overlap(&second_range, &first_range);

        if first_contains || second_contains {
            count[0] += 1;
        }

        if first_overlap || second_overlap {
            count[1] += 1;
        }
    }

    println!("Results ([P1, P2]): {:?}", count);
}
