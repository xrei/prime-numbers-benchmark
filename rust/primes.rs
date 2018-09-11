extern crate rayon;
use primes::rayon::iter::IntoParallelIterator;
use primes::rayon::iter::ParallelIterator;

pub fn test (max: u64) {
  let mut count = 0;
  let mut i = 2;
  let mut vec = Vec::new();
  while count < max {
    if isPrime(i) {
      vec.push(i);
      count+=1;
    }
    i+=1;
  }
  // println!("{:?}", vec)
  // println!("Max primes: {}", max);
}

pub fn test_functional (max: u64) {
  let mut vec = Vec::new();
  for x in (1..max + 1).filter(|x| is_prime_functional(*x)) { 
    vec.push(x)
  }
  // println!("{:?}", vec);
  // println!("Max primes: {}", max);
}

// Credits to: https://github.com/polachok
pub fn test_functional_paralell (max: u64) {
  let vec: Vec<_> = (1..max + 1).into_par_iter().filter(|x| is_prime_functional(*x)).collect();
  // println!("{:?}", vec);
  //  println!("Max primes: {}", vec.len());
}

fn isPrime(n: u64) -> bool {
  if n % 1 != 0 || n < 2 {
    return false;
  }

  let mut i = 2;
  while i <= (n as f64).sqrt() as u64 {
    if n % i == 0 {
      return false;
    }
    i+=1
  }
  return true;
}

fn is_prime_functional(n: u64) -> bool {

    if n < 4 { 
        return true; 
    }
    if n % 2 == 0 || n % 3 == 0 { 
        return false; 
    }

    (1u64..).map(|x| 6 * x - 1)
            .take_while(|x| (x * x) <= n)
            .all( |x| n % x != 0 && n % (x + 2)!= 0)
}