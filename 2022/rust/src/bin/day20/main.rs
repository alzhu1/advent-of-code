use aoc2022::read_file;

fn get_coordinates(nums: &[(i64, usize)]) -> [i64; 3] {
    let mut ret = [0, 0, 0];
    let zero_index = nums
        .iter()
        .enumerate()
        .filter(|(_, n)| n.0 == 0)
        .map(|(i, _)| i)
        .max()
        .unwrap();

    for i in 1..=3 {
        ret[i - 1] = nums[(zero_index + 1000 * i) % nums.len()].0;
        println!("Coordinate at {}: {}", i * 1000, ret[i - 1]);
    }

    ret
}

fn simulate(nums: &mut Vec<(i64, usize)>, nums_order: &[(i64, usize)], decryption_key: i64) {
    for num in nums_order.iter() {
        let i = nums
            .iter()
            .enumerate()
            .filter(|(_, n)| *n == num)
            .map(|(i, _)| i)
            .max()
            .unwrap();
        nums.remove(i);
        // let next = (i as i64 + num.0).rem_euclid(nums.len() as i64) as usize;

        let temp = (num.0 * decryption_key).rem_euclid(nums.len() as i64);
        let next = (i + temp as usize) % nums.len();

        nums.insert(next, *num);
    }
}

fn main() {
    let lines = read_file(file!(), "input.txt");

    let mut nums: Vec<(i64, usize)> = lines
        .map_while(Result::ok)
        .enumerate()
        .map(|(i, l)| (l.parse::<i64>().unwrap(), i))
        .collect();
    let nums_order = nums.clone();

    simulate(&mut nums, &nums_order, 1);

    let result: i64 = get_coordinates(&nums).iter().sum();
    println!("Part 1: {}", result);

    let mut nums_p2 = nums_order.clone();
    let decryption_key = 811589153;
    for _ in 0..10 {
        simulate(&mut nums_p2, &nums_order, decryption_key);
    }

    let result: i64 = get_coordinates(&nums_p2).iter().sum();
    println!("Part 2: {}", result * decryption_key);
}
