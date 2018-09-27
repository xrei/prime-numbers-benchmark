# Prime Numbers Benchmark (C)

## Single-Thread
[calc-prime.c](https://github.com/romshark/prime-numbers-benchmark/tree/master/c/calc-prime.c)
computes the prime numbers up until the given target using the square-root
calculation method in a single thread.

### Compiling
Please remember to compile the benchmark with the highest optimization level
`-O3` using any common C compiler, such as `gcc`, linking it to the math library
with `-lm`.
```
gcc -o prime-bench calc-prime.c -lm -O3
```

### Usage
First, compile the executable then execute it with 1 command line argument which
determines the target number.
```
./prime-bench 10000000
```


## Work-Stealing Thread-Pool
[calc-prime-concurrent.c](https://github.com/romshark/prime-numbers-benchmark/tree/master/c/calc-prime-concurrent.c)
computes the prime numbers up until the given target using the square-root
calculation method in a work-stealing thread pool.

### Compiling
Please remember to compile the benchmark with the highest optimization level
`-O3` using any common C compiler, such as `gcc`, linking it to the math (`-lm`)
and POSIX threads (`-lpthread`) libraries.
```
gcc -o prime-bench-conc calc-prime-concurrent.c -lm -lpthread -O3
```

### Usage
First, compile the executable then execute it with 2 command line arguments,
where the first determines the target number while the second determines the
size of the thread pool (ideally, use the number of available CPU cores).
```
./prime-bench-conc 10000000 12
```