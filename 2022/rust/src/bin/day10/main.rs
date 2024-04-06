use aoc2022::read_file;

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut cycles = 0;
    let mut x = 1;

    let mut signals = [20, 60, 100, 140, 180, 220].into_iter().peekable();
    let mut strength = 0;
    let mut sprite_positions: Vec<i32> = vec![];

    for line in lines.map_while(Result::ok) {
        let mut line_iter = line.split_whitespace();

        match line_iter.next().unwrap() {
            "noop" => {
                cycles += 1;
                sprite_positions.push(x);
            }
            "addx" => {
                cycles += 2;
                sprite_positions.push(x);
                sprite_positions.push(x);

                if cycles >= signals.peek().copied().unwrap_or(i32::MAX) {
                    let curr_signal = signals.next().unwrap();
                    strength += curr_signal * x;
                }

                x += line_iter.next().unwrap().parse::<i32>().unwrap();
            }
            _ => unreachable!(),
        };
    }

    let mut picture = String::new();
    for (i, sprite_pos) in sprite_positions.iter().enumerate() {
        let comp = (i % 40) as i32;

        if comp == 0 {
            picture.push('\n');
        }

        if comp >= sprite_pos - 1 && comp <= sprite_pos + 1 {
            picture.push('#');
        } else {
            picture.push('.');
        }
    }

    println!("Part 1: {}", strength);
    println!("Part 2: {}", picture);
}
