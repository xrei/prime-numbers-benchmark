package main

import (
	"flag"
	"fmt"
	"math"
	"sync"
	"time"
)

// IsPrimeSqrt determines whether value is a prime number using a square-root
// calculation
func IsPrimeSqrt(value uint) bool {
	for i := uint(2); i <= uint(math.Floor(
		math.Sqrt(float64(value)),
	)); i++ {
		if value%i == 0 {
			return false
		}
	}
	return value > 1
}

// ConcurrentPrimeCalculator represents a concurrent prime number calculator
// implemented by a work-stealing thread pool
type ConcurrentPrimeCalculator struct {
	target             uint
	progress           uint
	stateLock          *sync.Mutex
	numbersLock        *sync.Mutex
	numbers            []uint
	goroutinePoolWidth uint
}

// NewConcurrentCalculator creates a new concurrent prime number calculator
func NewConcurrentCalculator(
	target,
	parallel uint,
) *ConcurrentPrimeCalculator {
	return &ConcurrentPrimeCalculator{
		target:             target,
		progress:           0,
		stateLock:          &sync.Mutex{},
		numbersLock:        &sync.Mutex{},
		numbers:            make([]uint, 0, 10),
		goroutinePoolWidth: parallel,
	}
}

// Run starts the calculation blocking until all goroutines finished
func (c *ConcurrentPrimeCalculator) Run() []uint {
	wg := sync.WaitGroup{}
	wg.Add(int(c.goroutinePoolWidth))

	for i := uint(0); i < c.goroutinePoolWidth; i++ {
		go func() {
			for {
				var currentProgress uint
				// Synchronize access to calculator state
				c.stateLock.Lock()
				c.progress++
				currentProgress = c.progress
				c.stateLock.Unlock()

				// Abort when target is reached
				if currentProgress > c.target {
					wg.Done()
					return
				}

				if IsPrimeSqrt(currentProgress) {
					// Synchronize access to number slice
					c.numbersLock.Lock()
					c.numbers = append(c.numbers, currentProgress)
					c.numbersLock.Unlock()
				}
			}
		}()
	}
	wg.Wait()
	return c.numbers
}

var target = flag.Uint("target", 1000, "calculation target")
var parallel = flag.Uint("p", 1, "parallel workers")

func main() {
	flag.Parse()

	// Initialize calculator
	calc := NewConcurrentCalculator(*target, *parallel)

	// Start timer
	start := time.Now()

	// Perform calculation
	results := calc.Run()

	// Stop timer
	elapsed := time.Since(start)

	// Print results
	fmt.Println("target:       ", *target)
	fmt.Println("prime numbers:", len(results))
	fmt.Println("time:         ", elapsed)
}
