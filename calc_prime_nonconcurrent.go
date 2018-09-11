package main

import (
	"flag"
	"fmt"
	"math"
	"time"
)

// IsPrime determines whether value is a prime number using a naive loop
func IsPrime(value uint64) bool {
	for i := uint64(2); i <= uint64(math.Floor(float64(value)/2)); i++ {
		if value%i == 0 {
			return false
		}
	}
	return value > 1
}

// IsPrimeSqrt determines whether value is a prime number
// using a square-root calculation
func IsPrimeSqrt(value uint64) bool {
	for i := uint64(2); i <= uint64(math.Floor(math.Sqrt(float64(value)))); i++ {
		if value%i == 0 {
			return false
		}
	}
	return value > 1
}

// Naive naivly calculates the prime numbers until max
func Naive(max uint64) []uint64 {
	result := make([]uint64, 0, max)
	for i := uint64(1); i <= max; i++ {
		if IsPrime(i) {
			result = append(result, i)
		}
	}
	return result
}

// BySqrt calculates the prime numbers until max using square-root calculation
func BySqrt(max uint64) []uint64 {
	result := make([]uint64, 0, max)
	for i := uint64(1); i <= max; i++ {
		if IsPrimeSqrt(i) {
			result = append(result, i)
		}
	}
	return result
}

// SieveOfEratosthenes represents a rather advanced,
// optimized prime calculation method
func SieveOfEratosthenes(max uint64) []uint64 {
	result := make([]uint64, 0, max)
	f := make([]bool, max)
	for i := uint64(2); i <= uint64(math.Sqrt(float64(max))); i++ {
		if f[i] == false {
			for j := i * i; j < max; j += i {
				f[j] = true
			}
		}
	}
	for i := uint64(2); i < max; i++ {
		if f[i] == false {
			result = append(result, i)
		}
	}
	return result
}

var maxNum = flag.Uint64("maxnum", 100, "maximum number")
var naiveMethod = flag.Bool("naiv", false, "naive method enabled")
var squareRootMethod = flag.Bool("sqrt", false, "square-root method enabled")
var sieveOfEratosthenesMethod = flag.Bool(
	"sera",
	false,
	"sieve-of-eratosthenes method enabled",
)

func main() {
	flag.Parse()

	if *naiveMethod {
		start := time.Now()
		result := Naive(*maxNum)
		elapsed := time.Since(start)
		fmt.Println("\nisPrime:")
		fmt.Println("results: ", result)
		fmt.Println("time:    ", elapsed)
	}

	if *squareRootMethod {
		start := time.Now()
		result := BySqrt(*maxNum)
		elapsed := time.Since(start)
		fmt.Println("\nisPrimeSqrt:")
		fmt.Println("results: ", result)
		fmt.Println("time:    ", elapsed)
	}

	if *sieveOfEratosthenesMethod {
		start := time.Now()
		result := SieveOfEratosthenes(*maxNum)
		elapsed := time.Since(start)
		fmt.Println("\nsieveOfEratosthenes:")
		fmt.Println("results: ", result)
		fmt.Println("time:    ", elapsed)
	}
}
