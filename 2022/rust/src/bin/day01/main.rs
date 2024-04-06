use aoc2022::read_file;

fn main() {
    let lines = read_file(file!(), "input.txt");

    let mut calories = Vec::<u32>::new();
    calories.push(0);

    for line in lines.map_while(Result::ok) {
        match line.len() {
            0 => calories.push(0),
            _ => {
                let last = calories.last_mut().unwrap();
                *last += line.parse::<u32>().unwrap();
            }
        }
    }

    let mut largest: [u32; 3] = [*calories.iter().max().unwrap(), 0, 0];
    for i in 1..3 {
        largest[i] = *calories
            .iter()
            .filter(|&&x| x < largest[i - 1])
            .max()
            .unwrap();
    }

    println!("Part 1: {}", largest[0]);
    println!("Part 2: {}", largest.iter().sum::<u32>());
}
