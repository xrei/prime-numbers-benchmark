const maxPrimes = 10000
let primes = []

let calc = () => {
  console.time('Elapsed Time')
  let count = 0
  let i = 2

  function isPrime(n) {
    if (n % 1 || n < 2) return false
    let q = Math.sqrt(n)
  
    for (let i = 2; i <= q; i++) {
      if (n % i === 0) {
        return false
      }
    }
    return true
  }

  while (count < maxPrimes) {
    if (isPrime(i)) {
      primes.push(i);
      count++
    }
    i++
  }

  console.timeEnd('Elapsed Time')
  return primes.toString()
}

calc()
