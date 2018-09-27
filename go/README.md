# Prime Numbers Benchmark (Go)

## Work-Stealing Goroutine-Pool
This benchmarks computes the prime numbers up until the given target using the
square-root calculation method in a work-stealing goroutine pool.

### Usage
Just run the file with 2 command line arguments, where the `-target` determines
the target number while `-p` determines the size of the goroutine pool (ideally,
use the number of available CPU cores).

```
go run calc_prime.go -target 10000000 -p 12
```