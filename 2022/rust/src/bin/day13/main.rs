use std::cmp::Ordering;

use aoc2022::read_file;

type ListMappingType = Vec<(Option<u8>, Vec<usize>)>;

#[derive(Debug)]
struct ListMapping(ListMappingType);

impl std::str::FromStr for ListMapping {
    type Err = Box<dyn std::error::Error>;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut v: ListMappingType = Vec::new();
        let mut usizes: Vec<usize> = Vec::new();

        if s == "[]" {
            v.push((None, vec![]));
            return Ok(ListMapping(v));
        }

        let mut curr: u8 = u8::MAX;
        usizes.push(0);
        for (prev_byte, byte) in s.as_bytes().iter().zip(s.as_bytes().iter().skip(1)) {
            match *byte {
                b'[' => {
                    usizes.push(0);
                    curr = u8::MAX;
                }
                b',' => {
                    (*usizes.last_mut().unwrap()) += 1;
                }
                b']' => {
                    usizes.pop();

                    if curr == u8::MAX && *prev_byte == b'[' && !usizes.is_empty() {
                        v.push((None, usizes.clone()));
                    }
                }
                _ => {
                    curr = *byte - b'0';
                    if *prev_byte >= b'0' && *prev_byte <= b'9' {
                        curr += v.last().unwrap().0.unwrap() * 10;
                        v.pop();
                    }

                    v.push((Some(curr), usizes.clone()));
                }
            }
        }

        Ok(ListMapping(v))
    }
}

fn compare(left: &ListMappingType, right: &ListMappingType) -> Ordering {
    let mut left = left.clone();
    let mut right = right.clone();

    let left_is_value = left.len() == 1 && left[0].1.is_empty();
    let right_is_value = right.len() == 1 && right[0].1.is_empty();

    match (left_is_value, right_is_value) {
        (true, true) => {
            // These are either a number or an empty list, compare
            return left[0].0.cmp(&right[0].0);
        }
        (true, false) => {
            if left[0].0.is_none() {
                return Ordering::Less;
            }
            left[0].1.push(0);
        }
        (false, true) => {
            if right[0].0.is_none() {
                return Ordering::Greater;
            }
            right[0].1.push(0);
        }
        _ => (),
    }

    let mut left_copy: Vec<ListMappingType> = Vec::new();
    for (num, idxs) in &left {
        let mut v = idxs.clone();
        let item = v.remove(0);

        if item >= left_copy.len() {
            left_copy.push(Vec::new());
        }

        left_copy[item].push((*num, v));
    }

    let mut right_copy: Vec<ListMappingType> = Vec::new();
    for (num, idxs) in &right {
        let mut v = idxs.clone();
        let item = v.remove(0);

        if item >= right_copy.len() {
            right_copy.push(Vec::new());
        }

        right_copy[item].push((*num, v));
    }

    let mut i = 0;
    loop {
        if i >= left_copy.len() || i >= right_copy.len() {
            break;
        }

        match compare(&left_copy[i], &right_copy[i]) {
            Ordering::Equal => (),
            result => return result,
        }

        i += 1;
    }

    // If we leave loop, then the smaller one is the one with less items
    left.len().cmp(&right.len())
}

fn main() {
    let lines = read_file(file!(), "input.txt");

    let line_vec: Vec<String> = lines
        .map_while(Result::ok)
        .filter(|x| !x.is_empty())
        .collect();

    let mut mappings: Vec<ListMapping> = line_vec
        .clone()
        .into_iter()
        .chain(std::iter::once(String::from("[[2]]")).chain(std::iter::once(String::from("[[6]]"))))
        .map(|x| x.clone().parse().unwrap())
        .collect();

    let pairs: Vec<(ListMapping, ListMapping)> = line_vec
        .chunks(2)
        .map(|x| (x[0].clone().parse().unwrap(), x[1].clone().parse().unwrap()))
        .collect();

    let idx_sum: usize = pairs
        .iter()
        .enumerate()
        .map(
            |(i, (ListMapping(p1), ListMapping(p2)))| match compare(p1, p2) {
                Ordering::Less => i + 1,
                _ => 0,
            },
        )
        .sum();

    println!("Part 1: {}", idx_sum);

    mappings.sort_by(|ListMapping(a), ListMapping(b)| compare(a, b));

    let signal = mappings
        .iter()
        .enumerate()
        .map(|(i, ListMapping(m))| {
            if m.len() == 1
                && m[0].1.len() == 2
                && m[0]
                    .0
                    .and_then(|x| if x == 2 || x == 6 { Some(x) } else { None })
                    .is_some()
            {
                return i + 1;
            }

            1
        })
        .reduce(|acc, e| acc * e)
        .unwrap();

    println!("Part 2: {}", signal);
}
