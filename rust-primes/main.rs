mod primes;

use std::time::{Instant};

fn main() {
  let start = Instant::now();
  primes::test(100000);
  let duration = start.elapsed();
  println!("Time elapsed: {:?}", duration);
}
