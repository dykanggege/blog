use std::io;
use rand::Rng;

fn main() {
    println!("Hello, world!");
    println!("please input your guess.");


    let mut guess = String::new();

    io::stdin().read_line(&mut guess).expect("Fail to read line");

    println!("this is you guessed: {}",guess);

    let secret_num = rand::thread_rng().gen_range(1,101);
}

