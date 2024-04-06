use std::{cmp::max, collections::HashMap};

use aoc2022::read_file;

#[derive(Debug)]
struct Blueprint {
    ore_robot_cost: u8,
    clay_robot_cost: u8,
    obsidian_robot_cost: (u8, u8),
    geode_robot_cost: (u8, u8),
}

impl Blueprint {
    fn new(costs: Vec<Vec<u8>>) -> Self {
        let mapped_costs: Vec<(u8, u8)> = costs.iter().skip(2).map(|v| (v[0], v[1])).collect();
        Blueprint {
            ore_robot_cost: costs[0][0],
            clay_robot_cost: costs[1][0],
            obsidian_robot_cost: mapped_costs[0],
            geode_robot_cost: mapped_costs[1],
        }
    }

    fn get_max_ore_cost(&self) -> u8 {
        max(
            self.ore_robot_cost,
            max(
                self.clay_robot_cost,
                max(self.obsidian_robot_cost.0, self.geode_robot_cost.0),
            ),
        )
    }
}

// Applying some ideas from https://www.youtube.com/watch?v=5rb0vvJ7NCY, a great resource on how to optimize
#[derive(Clone, Debug, PartialEq, Eq, Hash)]
struct Pack {
    resources: (u8, u8, u8),
    robots: (u8, u8, u8, u8),
}

#[derive(Clone, Debug, PartialEq, Eq, Hash)]
struct State {
    minutes: u8,
    pack: Pack,
}

impl State {
    fn default(minutes: u8) -> Self {
        State {
            minutes,
            pack: Pack {
                resources: (0, 0, 0),
                robots: (1, 0, 0, 0),
            },
        }
    }

    fn new(minutes: u8, resources: (u8, u8, u8), robots: (u8, u8, u8, u8)) -> Self {
        State {
            minutes,
            pack: Pack { resources, robots },
        }
    }

    fn simulate(
        self,
        blueprint: &Blueprint,
        cache: &mut HashMap<Pack, (u8, u16)>,
        curr_geodes: u16,
        best_geodes: &mut u16,
    ) {
        // If in cache, ignore this state
        if cache.contains_key(&self.pack) {
            let (cache_minutes, cache_geodes) = cache[&self.pack];
            if cache_geodes >= curr_geodes || cache_minutes >= self.minutes {
                return;
            }
        }

        // Let's explore
        let minutes = self.minutes;
        let (ore, clay, obsidian) = self.pack.resources;
        let (ore_robots, clay_robots, obsidian_robots, geode_robots) = self.pack.robots;

        if minutes == 0 {
            if curr_geodes > *best_geodes {
                *best_geodes = curr_geodes;
                cache.insert(self.pack.clone(), (self.minutes, curr_geodes));
            }
            return;
        }

        let next_ore = ore + ore_robots;
        let next_clay = clay + clay_robots;
        let next_obsidian = obsidian + obsidian_robots;
        let next_geodes = curr_geodes + geode_robots as u16;

        // Imagine we create geode robots every single minute
        // If that's not enough to beat best_geodes, stop exploring
        let max_geodes_delta = minutes as u16 * ((geode_robots + ((minutes - 1) << 1)) as u16);
        if next_geodes + max_geodes_delta < *best_geodes {
            return;
        }

        // Robot building check
        // Should not need to build robot if we have enough robots for this resource
        if ore >= blueprint.ore_robot_cost && ore_robots < blueprint.get_max_ore_cost() {
            State::new(
                minutes - 1,
                (
                    next_ore - blueprint.ore_robot_cost,
                    next_clay,
                    next_obsidian,
                ),
                (ore_robots + 1, clay_robots, obsidian_robots, geode_robots),
            )
            .simulate(blueprint, cache, next_geodes, best_geodes);
        }

        if ore >= blueprint.clay_robot_cost && clay_robots < blueprint.obsidian_robot_cost.1 {
            State::new(
                minutes - 1,
                (
                    next_ore - blueprint.clay_robot_cost,
                    next_clay,
                    next_obsidian,
                ),
                (ore_robots, clay_robots + 1, obsidian_robots, geode_robots),
            )
            .simulate(blueprint, cache, next_geodes, best_geodes);
        }

        if ore >= blueprint.obsidian_robot_cost.0
            && clay >= blueprint.obsidian_robot_cost.1
            && obsidian_robots < blueprint.geode_robot_cost.1
        {
            State::new(
                minutes - 1,
                (
                    next_ore - blueprint.obsidian_robot_cost.0,
                    next_clay - blueprint.obsidian_robot_cost.1,
                    next_obsidian,
                ),
                (ore_robots, clay_robots, obsidian_robots + 1, geode_robots),
            )
            .simulate(blueprint, cache, next_geodes, best_geodes);
        }

        if ore >= blueprint.geode_robot_cost.0 && obsidian >= blueprint.geode_robot_cost.1 {
            State::new(
                minutes - 1,
                (
                    next_ore - blueprint.geode_robot_cost.0,
                    next_clay,
                    next_obsidian - blueprint.geode_robot_cost.1,
                ),
                (ore_robots, clay_robots, obsidian_robots, geode_robots + 1),
            )
            .simulate(blueprint, cache, next_geodes, best_geodes);
        }

        // And simulate the do-nothing case
        State::new(
            minutes - 1,
            (next_ore, next_clay, next_obsidian),
            (ore_robots, clay_robots, obsidian_robots, geode_robots),
        )
        .simulate(blueprint, cache, next_geodes, best_geodes);

        cache.insert(self.pack, (self.minutes, *best_geodes));
    }
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut total_quality = 0;
    let mut product = 1;

    for (i, line) in lines.map_while(Result::ok).enumerate() {
        let costs: Vec<Vec<u8>> = line
            .split("costs")
            .skip(1)
            .map(|l| {
                l.split_whitespace()
                    .filter_map(|s| s.parse::<u8>().ok())
                    .collect::<Vec<u8>>()
            })
            .collect();

        let blueprint = Blueprint::new(costs);

        let mut best_geodes = 0;
        State::default(24).simulate(&blueprint, &mut HashMap::new(), 0, &mut best_geodes);

        println!(
            "Simulation for blueprint {} returned {} geodes",
            i + 1,
            best_geodes
        );

        total_quality += (best_geodes as usize) * (i + 1);

        if i < 3 {
            println!("Start simulation for 32 min of blueprint {}", i + 1);
            best_geodes = 0;
            State::default(32).simulate(&blueprint, &mut HashMap::new(), 0, &mut best_geodes);
            println!(
                "32 min simulation for blueprint {} returned {} geodes",
                i + 1,
                best_geodes
            );

            product *= best_geodes;
        }
    }

    println!("Part 1: {}", total_quality);
    println!("Part 2: {}", product);
}
