mod primes;

use std::time::{Instant};

fn main() {
  let start = Instant::now();
  // primes::test(1000000); // this is very slow!
  primes::test_functional(1000000); // this is somewhat fast.
  let duration = start.elapsed();
  println!("Time elapsed: {:?}", duration);
}
