
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