use std::fs::File;
use std::io::{self, BufRead};
use std::path::PathBuf;

pub fn read_file(file_base: &str, filename: &str) -> io::Lines<io::BufReader<File>> {
    let mut file_path = PathBuf::from(file_base);
    file_path.pop();
    file_path.push(filename);

    println!("{}", file_path.to_str().unwrap());

    let file_res = File::open(file_path);
    match file_res {
        Ok(file) => io::BufReader::new(file).lines(),
        Err(_) => panic!("Could not read file {}", filename),
    }
}
