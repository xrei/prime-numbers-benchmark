mod primes;

use std::time::{Instant};

fn main() {
  let start = Instant::now();
  // primes::test(1000000); // this is very slow!
  // 45 sec for 1kk primes
  primes::test_functional(1000000); // this is somewhat fast.
  // ~1 sec for 1kk primes
  let duration = start.elapsed();
  println!("Time elapsed: {:?}", duration);
}
