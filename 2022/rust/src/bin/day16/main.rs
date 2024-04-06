use std::collections::{HashMap, HashSet, VecDeque};

use aoc2022::read_file;

#[derive(Debug, Clone)]
struct Valve {
    key: String,
    id: u16,
    flow: u16,
    paths: Vec<String>,
    weighted_paths: HashMap<u16, u8>,
}

impl Valve {
    fn add_weighted_path(&mut self, id: u16, dist: u8) {
        self.weighted_paths.insert(id, dist);
    }
}

#[derive(Clone, Debug, PartialEq, Eq, PartialOrd, Ord, Hash)]
struct State {
    curr_loc_id: u16,
    minutes: u8,
    valves_on: u16,
    flow_max: u16,
}

impl State {
    fn search(
        self,
        curr_score: u16,
        best_score: &mut u16,
        valve_map: &HashMap<u16, Valve>,
        target: u16,
        best_for_target: &mut HashMap<u16, u16>,
    ) {
        // Impressively, this does better without the overhead of keeping a cache

        // If valves_on matches target, we are done
        if self.valves_on == target {
            if *best_score < curr_score {
                *best_score = curr_score;
                *best_for_target.entry(target).or_insert(0) = *best_score;
            }
            return;
        }

        // Next, if minutes left * flow_max is less than our best score, ignore this state
        // (No point in searching)
        if curr_score + (u16::from(self.minutes) * self.flow_max) <= *best_score {
            return;
        }

        // Let's explore: for each weighted path of a valve, go to each one and pull
        let valve = &valve_map[&self.curr_loc_id];
        let mut cannot_explore_count = 0;

        for (next_id, travel_time) in valve.weighted_paths.iter() {
            let valid_valve = (next_id & target) > 0;
            let off = (next_id & self.valves_on) == 0;

            if valid_valve && off && *travel_time < self.minutes {
                let next_valve = &valve_map[next_id];

                // Account for moving there + turning on valve
                let next_minutes = self.minutes - (travel_time + 1);
                let next_score = curr_score + (next_valve.flow * u16::from(next_minutes));

                let next_state = State {
                    curr_loc_id: *next_id,
                    minutes: next_minutes,
                    valves_on: self.valves_on | next_id,
                    flow_max: self.flow_max - next_valve.flow,
                };

                next_state.search(next_score, best_score, valve_map, target, best_for_target);
            } else {
                cannot_explore_count += 1;
            }
        }

        if cannot_explore_count == valve.weighted_paths.len() && *best_score < curr_score {
            *best_score = curr_score;
            *best_for_target.entry(target).or_insert(0) = *best_score;
        }
    }
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut valve_map: HashMap<String, Valve> = HashMap::new();

    let mut id_idx = 0;

    for line in lines.map_while(Result::ok) {
        let parts: Vec<&str> = line
            .split_whitespace()
            .enumerate()
            .filter(|(i, _)| *i == 1 || *i == 4 || *i > 8)
            .map(|x| x.1)
            .collect();

        let key = String::from(parts[0]);
        let flow = parts[1][5..parts[1].len() - 1].parse::<u16>().unwrap();
        let paths = parts
            .iter()
            .skip(2)
            .map(|x| String::from(if x.len() != 2 { &x[0..2] } else { *x }))
            .collect::<Vec<String>>();

        let id = match (key.clone(), flow) {
            (s, 0) if s == "AA" => 0,
            (_, f) if f > 0 => {
                id_idx += 1;
                1 << (id_idx - 1)
            }
            _ => 1,
        };

        valve_map.insert(
            key.clone(),
            Valve {
                key,
                id,
                flow,
                paths,
                weighted_paths: HashMap::new(),
            },
        );
    }

    // Do BFS from each non-zero flow valve to determine path lens to all others
    let mut valid_valve_map: HashMap<u16, Valve> = valve_map
        .iter()
        .map(|v| v.1)
        .cloned()
        .map(|v| (v.id, v))
        .filter(|(_, valve)| valve.key == "AA" || valve.flow > 0)
        .collect();

    for (_, valve) in valid_valve_map.iter_mut() {
        if valve.key != "AA" && valve.flow == 0 {
            continue;
        }

        let mut queue: VecDeque<(String, u8)> = VecDeque::new();
        let mut cache: HashSet<String> = HashSet::new();
        queue.push_back((valve.key.clone(), 0));

        while let Some((curr_valve_key, curr_dist)) = queue.pop_front() {
            if cache.contains(&curr_valve_key) {
                continue;
            }
            cache.insert(curr_valve_key.clone());

            let curr_valve = &valve_map[&curr_valve_key];

            if curr_valve.key != valve.key && curr_valve.flow > 0 {
                valve.add_weighted_path(curr_valve.id, curr_dist);
            }

            for p in curr_valve.paths.iter() {
                queue.push_back((p.clone(), curr_dist + 1));
            }
        }
    }

    println!("Starting search");
    let mut state = State {
        curr_loc_id: 0,
        minutes: 30,
        valves_on: 0,
        flow_max: valid_valve_map.iter().map(|x| x.1.flow).sum(),
    };

    let mut best_score = 0;
    let target = (1 << (valid_valve_map.len() - 1)) - 1;
    state.clone().search(
        0,
        &mut best_score,
        &valid_valve_map,
        target,
        &mut HashMap::new(),
    );

    println!("Part 1: {}", best_score);

    state.minutes = 26;
    let mut best_for_target = HashMap::new();
    for t in 0..=target {
        let flow_max: u16 = (0..=15)
            .map(|x| {
                let id = (1 << x) & t;

                match valid_valve_map.get(&id) {
                    Some(v) => v.flow,
                    None => 0,
                }
            })
            .sum();

        best_for_target.insert(t, 0);
        state.flow_max = flow_max;
        state
            .clone()
            .search(0, &mut 0, &valid_valve_map, t, &mut best_for_target);
    }

    let best_pair_score = (0..=target)
        .map(|t| {
            let best_player = best_for_target[&t];
            let best_elephant = best_for_target[&((!t) & target)];

            best_player + best_elephant
        })
        .max()
        .unwrap();

    println!("Part 2: {}", best_pair_score);
}
