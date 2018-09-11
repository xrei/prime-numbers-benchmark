mod primes;

use std::time::{Instant};

fn main() {
  let start = Instant::now();
  // primes::test(1000000); // this is very slow!
  // 45 sec for 1kk primes
  primes::test_functional(10000000); // this is somewhat fast.
  // ~1 sec for 1kk primes
  // primes::test_functional_paralell(1000000);
  // 26 ms WHAT TA FORK BOIS, ZIS WAS SO FAST
  let duration = start.elapsed();
  println!("Time elapsed: {:?}", duration);
}
