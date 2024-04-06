use aoc2022::read_file;

fn score_match(opponent: i16, play: i16) -> i16 {
    match play {
        lose if (opponent + 2) % 3 == lose => 0,
        draw if opponent == draw => 3,
        win if (opponent + 1) % 3 == win => 6,
        _ => unreachable!(),
    }
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut scores = [0, 0];

    for line in lines.map_while(Result::ok) {
        let pair: Vec<&str> = line.split(' ').collect();

        let opponent: i16 = i16::from(pair[0].as_bytes()[0] - b'A');

        let (play1, play2) = match pair[1] {
            "X" => (0, (opponent + 2) % 3),
            "Y" => (1, opponent),
            "Z" => (2, (opponent + 1) % 3),
            _ => unreachable!(),
        };

        scores[0] += play1 + 1 + score_match(opponent, play1);
        scores[1] += play2 + 1 + score_match(opponent, play2);
    }

    println!("Results ([P1, P2]): {:?}", scores);
}
