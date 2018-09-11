package main

import (
	"fmt"
	"time"
)

func Generate(ch chan<- int) {
	for i := 2; ; i++ {
		ch <- i
	}
}

func Filter(in <-chan int, out chan<- int, prime int) {
	for {
		i := <-in
		if i%prime != 0 {
			out <- i
		}
	}
}

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {
	start := time.Now()

	var MAX_PRIMES = 10000

	var arr []int
	ch := make(chan int)
	go Generate(ch)
	for i := 0; i < MAX_PRIMES; i++ {
		prime := <-ch
		arr = append(arr, prime)
		ch1 := make(chan int)
		go Filter(ch, ch1, prime)
		ch = ch1
	}

	fmt.Println(arr)
	t := time.Now()
	fmt.Println("Time elapsed", t.Sub(start))
}
