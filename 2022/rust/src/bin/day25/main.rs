use aoc2022::read_file;

fn from_snafu(s: String) -> i64 {
    let mut num = 0;
    for (place, c) in s.as_bytes().iter().rev().enumerate() {
        let value = match *c {
            b'=' => -2,
            b'-' => -1,
            v => (v - b'0') as i64
        };

        num += 5_i64.pow(place as u32) * value;
    }

    num
}

fn to_snafu(mut n: i64) -> String {
    let mut place = 0;
    while 5_i64.pow(place) <= n {
        place += 1;
    }
    place -= 1;

    let mut v: Vec<u8> = vec!();    
    loop {
        let curr_exp = 5_i64.pow(place);
        let value = n / curr_exp;
        v.push(value.to_string().as_bytes()[0]);

        n %= curr_exp;

        if place == 0 {
            break;
        }

        place -= 1;
    }

    for i in (0..v.len()).rev() {
        let curr = v[i];
        let carry = match curr {
            b'3' => {
                v[i] = b'=';
                true
            },
            b'4' => {
                v[i] = b'-';
                true
            },
            b'5' => {
                v[i] = b'0';
                true
            },
            _ => false
        };

        if carry {
            if i == 0 {
                v.insert(0, b'1');
            } else {
                let next_place = v[i - 1] + 1;
                v[i - 1] = next_place;
            }
        }
    }

    String::from_utf8(v).unwrap()
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let sum: i64 = lines.map_while(Result::ok).map(from_snafu).sum();

    println!("Sum: {}, SNAFU: {}", sum, to_snafu(sum));
}
