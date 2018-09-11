package main

import (
	"flag"
	"fmt"
	"math"
	"sync"
	"time"
)

// IsPrimeSqrt determines whether value is a prime number
// using a square-root calculation
func IsPrimeSqrt(value uint64) bool {
	for i := uint64(2); i <= uint64(math.Floor(
		math.Sqrt(float64(value)),
	)); i++ {
		if value%i == 0 {
			return false
		}
	}
	return value > 1
}

// Compute calculates the prime numbers until max using the square-root method
func Compute(max uint64) []uint64 {
	result := make([]uint64, 0, max)
	for i := uint64(1); i <= max; i++ {
		if IsPrimeSqrt(i) {
			result = append(result, i)
		}
	}
	return result
}

// ComputeSqrtParallelNaive calculates the prime numbers until max
// using the square-root method each in a separate goroutine (naive concurrency)
func ComputeSqrtParallelNaive(max uint64) []uint64 {
	result := make([]uint64, 0, max)

	// resultLock protects result from concurrent access
	resultLock := sync.Mutex{}

	// waitGroup prevents this function from returning too early
	waitGroup := sync.WaitGroup{}
	waitGroup.Add(int(max))

	for i := uint64(1); i <= max; i++ {
		// Run every calculation in a separate goroutine
		go func() {
			if IsPrimeSqrt(i) {
				resultLock.Lock()
				result = append(result, i)
				resultLock.Unlock()
			}
			waitGroup.Done()
		}()
	}

	// Wait until all goroutines have finished their calculations
	waitGroup.Wait()
	return result
}

var max = flag.Uint64("max", 100, "maximum number")
var parallel = flag.Int64("p", 0, "parallel workers")
var verbose = flag.Bool("v", false, "verbose mode")

func main() {
	flag.Parse()
	start := time.Now()
	var results []uint64

	switch *parallel {
	case 0:
		results = Compute(*max)
	case -1:
		results = ComputeSqrtParallelNaive(*max)
	default:
		/* TODO: compute on goroutine pool */
		panic("Goroutine pool concurrency not yet implemented")
	}

	elapsed := time.Since(start)
	if *verbose {
		fmt.Println("results: ", results)
	}
	fmt.Println("time:    ", elapsed)
}
