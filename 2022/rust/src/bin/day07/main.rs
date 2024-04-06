use std::{collections::HashMap, path::PathBuf};

use aoc2022::read_file;

#[derive(Debug)]
struct Node {
    path: String,
    files: HashMap<String, u32>,
    dirs: Vec<String>,
}

impl Node {
    fn file_sum(&self) -> u32 {
        self.files.iter().map(|x| x.1).sum()
    }

    fn get_size(
        &self,
        node_map: &HashMap<String, Node>,
        small_size: &mut u32,
        sizes: &mut HashMap<String, u32>,
    ) -> u32 {
        let mut curr_sum = self.file_sum();

        for f in self.dirs.iter() {
            let mut path = PathBuf::from(&self.path);
            path.push(f);

            let key = path.into_os_string().into_string().unwrap();
            curr_sum += node_map[key.as_str()].get_size(node_map, small_size, sizes);
        }

        if curr_sum <= 100000 {
            (*small_size) += curr_sum;
        }

        sizes.insert(self.path.clone(), curr_sum);

        curr_sum
    }
}

fn main() {
    let lines = read_file(file!(), "input.txt");
    let mut curr_path = PathBuf::new();

    let mut node_map: HashMap<String, Node> = HashMap::new();

    for line in lines.map_while(Result::ok) {
        let mut line_iter = line.split_whitespace();

        if line.starts_with('$') {
            line_iter.next();

            if line_iter.next().is_some() {
                if let Some(path_change) = line_iter.next() {
                    // cd
                    match path_change {
                        ".." => {
                            curr_path.pop();
                        }
                        // Since inputs start with / and use it once, this fits
                        _ => curr_path.push(path_change),
                    };
                } else {
                    // ls, create node if not exists
                    let s = curr_path.clone().into_os_string().into_string().unwrap();
                    let n = Node {
                        path: s.clone(),
                        files: HashMap::new(),
                        dirs: vec![],
                    };

                    node_map.insert(s, n);
                }
            }

            continue;
        }

        let [first, second] = line_iter.collect::<Vec<&str>>()[..2] else {
            unreachable!()
        };
        let path = curr_path.clone().into_os_string().into_string().unwrap();
        let curr_node: &mut Node = node_map.get_mut(&path).unwrap();

        match first {
            "dir" => curr_node.dirs.push(second.to_string()),
            _ => {
                curr_node
                    .files
                    .insert(second.to_string(), first.parse::<u32>().unwrap());
            }
        }
    }

    let mut small_size: u32 = 0;
    let mut sizes: HashMap<String, u32> = HashMap::new();

    let root_sum = node_map["/"].get_size(&node_map, &mut small_size, &mut sizes);

    let size_needed = 30000000 - (70000000 - root_sum);
    let best_delete_size = sizes
        .iter()
        .map(|x| x.1)
        .filter(|&&x| x >= size_needed)
        .min()
        .unwrap();

    println!("Part 1: {}", small_size);
    println!("Part 2: {}", best_delete_size);
}
