#[derive(Copy, Clone, Debug)]
enum Op {
    Add(u64),
    Multiply(u64),
    Square,
}

trait CustomOp {
    fn operate(self, op: Op) -> u64;
}

impl CustomOp for u64 {
    fn operate(self, op: Op) -> u64 {
        match op {
            Op::Add(a) => self + a,
            Op::Multiply(m) => self * m,
            Op::Square => self * self,
        }
    }
}

struct Monkey {
    items: Vec<(u64, Vec<Op>)>,
    operation: Op,
    throw_test_params: (u64, usize, usize),
    inspect_count: u64,
}

impl Monkey {
    fn new(items: Vec<u64>, operation: Op, throw_test_params: (u64, usize, usize)) -> Self {
        Monkey {
            items: items.into_iter().map(|x| (x, Vec::new())).collect(),
            operation,
            throw_test_params,
            inspect_count: 0,
        }
    }

    fn process_items(&mut self) -> Vec<((u64, Vec<Op>), usize)> {
        let (divisor, a, b) = self.throw_test_params;
        self.inspect_count += self.items.len() as u64;

        self.items
            .drain(0..)
            .map(|(num, mut ops)| {
                // For part 1, divide by 3 as well

                // From a reddit hint, alternative is doing mod (product of all divisors)
                // Good optimization but I like my approach lol (did it before checking reddit)
                // let num = num.operate(self.operation) % 9699690;
                // let n = num;

                // Check if we can consolidate the last operation with this one
                match (ops.last_mut(), self.operation) {
                    (Some(Op::Add(last)), Op::Add(a)) => *last += a,
                    (Some(Op::Multiply(last)), Op::Multiply(m)) => *last *= m,
                    _ => ops.push(self.operation),
                }
                let n = ops.iter().fold(num, |acc, &op| acc.operate(op) % divisor);

                if n % divisor == 0 {
                    ((num, ops), a)
                } else {
                    ((num, ops), b)
                }
            })
            .collect()
    }
}

fn main() {
    let mut monkeys = vec![
        // Monkey::new(vec!(79, 98), Op::Multiply(19), (23, 2, 3)),
        // Monkey::new(vec!(54, 65, 75, 74), Op::Add(6), (19, 2, 0)),
        // Monkey::new(vec!(79, 60, 97), Op::Square, (13, 1, 3)),
        // Monkey::new(vec!(74), Op::Add(3), (17, 0, 1))
        Monkey::new(
            vec![54, 98, 50, 94, 69, 62, 53, 85],
            Op::Multiply(13),
            (3, 2, 1),
        ),
        Monkey::new(vec![71, 55, 82], Op::Add(2), (13, 7, 2)),
        Monkey::new(vec![77, 73, 86, 72, 87], Op::Add(8), (19, 4, 7)),
        Monkey::new(vec![97, 91], Op::Add(1), (17, 6, 5)),
        Monkey::new(
            vec![78, 97, 51, 85, 66, 63, 62],
            Op::Multiply(17),
            (5, 6, 3),
        ),
        Monkey::new(vec![88], Op::Add(3), (7, 1, 0)),
        Monkey::new(vec![87, 57, 63, 86, 87, 53], Op::Square, (11, 5, 0)),
        Monkey::new(vec![73, 59, 82, 65], Op::Add(6), (2, 4, 3)),
    ];

    for c in 0..10000 {
        if c > 0 && (c == 20 || c % 1000 == 0) {
            let max = monkeys.iter().map(|m| m.inspect_count).max().unwrap();
            let second_max = monkeys
                .iter()
                .map(|m| m.inspect_count)
                .filter(|&x| x < max)
                .max()
                .unwrap();

            println!("{}: {} * {} = {}", c, max, second_max, max * second_max);
        }

        for i in 0..monkeys.len() {
            for (worry, dest) in monkeys[i].process_items() {
                monkeys[dest].items.push(worry);
            }
        }
    }

    let max = monkeys.iter().map(|m| m.inspect_count).max().unwrap();
    let second_max = monkeys
        .iter()
        .map(|m| m.inspect_count)
        .filter(|&x| x < max)
        .max()
        .unwrap();

    println!("Part 2: {} * {} = {}", max, second_max, max * second_max);
}
