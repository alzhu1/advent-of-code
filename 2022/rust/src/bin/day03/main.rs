use aoc2022::read_file;
use std::collections::HashSet;

fn get_priority(priority: &u8) -> u8 {
    match *priority {
        lower if lower >= b'a' => lower - b'a' + 1,
        upper if upper >= b'A' => upper - b'A' + 27,
        _ => unreachable!(),
    }
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut priorities = [0, 0];
    let mut groups: Vec<String> = vec![];

    for line in lines.map_while(Result::ok) {
        let (first, second) = line.split_at(line.len() / 2);

        let set: HashSet<&u8> = HashSet::from_iter(first.as_bytes());
        let set2: HashSet<&u8> = HashSet::from_iter(second.as_bytes());
        let mut intersection = set.intersection(&set2);

        if let Some(priority) = intersection.next() {
            priorities[0] += i32::from(get_priority(priority));
        }

        groups.push(line);
        if groups.len() == 3 {
            let mut line_set: HashSet<&u8> = HashSet::from_iter(groups[0].as_bytes());

            for group in groups.iter() {
                let group_set: HashSet<&u8> = HashSet::from_iter(group.as_bytes());
                line_set = line_set.intersection(&group_set).copied().collect();
            }

            if let Some(priority) = line_set.drain().next() {
                priorities[1] += i32::from(get_priority(priority));
            }

            groups.clear();
        }
    }

    println!("Results ([P1, P2]): {:?}", priorities);
}
