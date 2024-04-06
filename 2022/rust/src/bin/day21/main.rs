use std::collections::HashMap;

use aoc2022::read_file;

#[derive(Clone, Copy, Debug)]
enum Job {
    Number(i64),
    Add,
    Subtract,
    Multiply,
    Divide,
    Equals,
}

#[derive(Debug)]
struct Monkey {
    key: String,
    job: Job,
    m1: String,
    m2: String,
    parent: String,
}

impl Monkey {
    fn get_parent_child_pair(&self) -> [(String, String); 2] {
        [
            (self.key.clone(), self.m1.clone()),
            (self.key.clone(), self.m2.clone()),
        ]
    }

    fn perform_job(
        &self,
        monkey_map: &HashMap<String, Monkey>,
        monkey_result: &mut HashMap<String, i64>,
    ) -> i64 {
        if monkey_result.contains_key(&self.key) {
            return monkey_result[&self.key];
        }

        if let Job::Number(num) = self.job {
            monkey_result.insert(self.key.clone(), num);
            return num;
        }

        let monkey1 = monkey_map
            .get(&self.m1)
            .unwrap()
            .perform_job(monkey_map, monkey_result);
        let monkey2 = monkey_map
            .get(&self.m2)
            .unwrap()
            .perform_job(monkey_map, monkey_result);

        let result = match self.job {
            Job::Number(num) => num,
            Job::Add => monkey1 + monkey2,
            Job::Subtract => monkey1 - monkey2,
            Job::Multiply => monkey1 * monkey2,
            Job::Divide => monkey1 / monkey2,
            Job::Equals => {
                if monkey1 == monkey2 {
                    1
                } else {
                    0
                }
            }
        };

        monkey_result.insert(self.key.clone(), result);
        result
    }
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut monkey_map: HashMap<String, Monkey> = HashMap::new();
    let mut monkey_result: HashMap<String, i64> = HashMap::new();

    for line in lines.map_while(Result::ok) {
        let (monkey_id, rest) = line.split_at(line.find(':').unwrap());
        let job_str = rest[2..].to_string();

        let mut m1 = String::from("");
        let mut m2 = String::from("");

        let job = match job_str.parse::<i64>() {
            Ok(num) => Job::Number(num),
            Err(_) => {
                let mut job_iter = job_str.split_whitespace();

                m1 = job_iter.next().unwrap().to_string();
                let operation = job_iter.next().unwrap();
                m2 = job_iter.next().unwrap().to_string();

                match operation {
                    "+" => Job::Add,
                    "-" => Job::Subtract,
                    "*" => Job::Multiply,
                    "/" => Job::Divide,
                    _ => unreachable!(),
                }
            }
        };

        monkey_map.insert(
            monkey_id.to_string(),
            Monkey {
                job,
                key: monkey_id.to_string(),
                m1,
                m2,
                parent: String::from(""),
            },
        );
    }

    let parent_child_pairs: Vec<[(String, String); 2]> = monkey_map
        .values()
        .map(|m| m.get_parent_child_pair())
        .collect();
    for [(p1, c1), (p2, c2)] in parent_child_pairs {
        if !c1.is_empty() && !c2.is_empty() {
            monkey_map.get_mut(&c1).unwrap().parent = p1;
            monkey_map.get_mut(&c2).unwrap().parent = p2;
        }
    }

    let result = monkey_map["root"].perform_job(&monkey_map, &mut monkey_result);
    println!("Part 1: {}", result);

    monkey_map.get_mut("root").unwrap().job = Job::Equals;

    // Store a list of (monkey key, monkey job, other branch result, other is on rhs)
    let mut human_to_top: Vec<(String, Job, i64, bool)> = vec![];
    let mut m = &monkey_map["humn"];
    loop {
        if let Job::Number(_) = m.job {
            human_to_top.push((m.key.clone(), m.job, 0, true));
        } else {
            let other = if m.m1 == human_to_top.last().unwrap().0 {
                (m.m2.clone(), true)
            } else {
                (m.m1.clone(), false)
            };
            human_to_top.push((m.key.clone(), m.job, monkey_result[&other.0], other.1));
        }

        if m.parent.is_empty() {
            break;
        }

        m = &monkey_map[&m.parent];
    }

    let human_num =
        human_to_top
            .iter()
            .rev()
            .fold(0, |acc, (_, job, other_result, other_is_rhs)| {
                // Do opposite
                match *job {
                    Job::Number(_) => acc,
                    Job::Add => acc - other_result,
                    Job::Subtract => {
                        if *other_is_rhs {
                            acc + other_result
                        } else {
                            other_result - acc
                        }
                    }
                    Job::Multiply => acc / other_result,
                    Job::Divide => {
                        if *other_is_rhs {
                            acc * other_result
                        } else {
                            other_result / acc
                        }
                    }
                    Job::Equals => *other_result,
                }
            });

    println!("Part 2: {}", human_num);

    monkey_map.get_mut("humn").unwrap().job = Job::Number(human_num);
    monkey_result.clear();

    let root = &monkey_map["root"];
    let result = monkey_map["root"].perform_job(&monkey_map, &mut monkey_result);

    println!(
        "Root: {}, {} == {}",
        result, monkey_result[&root.m1], monkey_result[&root.m2]
    );
}
