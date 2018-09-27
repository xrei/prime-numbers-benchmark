#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>

typedef enum { false, true } bool;

/* NUMBER VECTOR */

// vector_uint represents a vector of unsigned integers.
//
// use new_vector_uint() to create a new instance.
// use vector_uint_pushback() to append a new value
typedef struct {
	unsigned int* _data;
	size_t _capacity; // Total capacity
	size_t _width;    // Number of stored elements
} vector_uint;

// new_vector_uint allocates a new unsigned integer vector
vector_uint* new_vector_uint(const size_t capacity) {
	// Allocate structure on the heap
	vector_uint* new_vector = (vector_uint*) malloc(sizeof(vector_uint));
	if (!new_vector) {
		return NULL; // Failed to allocate a new vector
	}

	// Set initial state
	new_vector->_width = 0;
	new_vector->_capacity = capacity;

	if (capacity > 0) {
		// Preallocate memory
		new_vector->_data = (unsigned int*) malloc(
			sizeof(unsigned int) * capacity
		);
	}

	return new_vector; // Successfully initialized
}

// vector_uint_pushback appends a new value to the vector.
// Grows the vector if needed doubling it's capacity
int vector_uint_pushback(
	vector_uint* vector,
	const unsigned int new_value
) {
	size_t new_width = vector->_width + 1;
	size_t currentCap = vector->_capacity;
	if (new_width > currentCap) {
		// Reallocation the vector doubling its capacity
		if (currentCap < 1) {
			vector->_capacity = 2;
		} else {
			vector->_capacity *= 2;
		}
		vector->_data = realloc(
			vector->_data,
			sizeof(unsigned int) * vector->_capacity
		);
		if (!vector->_data) {
			return 1; // Couldn't grow vector
		}
	}

	vector->_data[new_width-1] = new_value;
	vector->_width = new_width;

	return 0; // Successfull push back
}

/* COMPUTE */

// is_prime_sqrt determines whether value is a prime number using a square-root
// calculation
bool is_prime_sqrt(const unsigned int value) {
	for (
		unsigned int i = 2;
		i <= floor(sqrt((double) value));
		++i
	) {
		if(value % i == 0) {
			return false;
		}
	}
	return value > 1;
}

// compute calculates the prime numbers until target using the square-root
// method returning them as a vector
vector_uint* compute(const unsigned int target) {
	vector_uint* numbers = new_vector_uint(0);

	for (unsigned int number = 1; number <= target; ++number) {
		if (is_prime_sqrt(number)) {
			vector_uint_pushback(numbers, number);
		}
	}

	return numbers;
}

void main(int argc, char *argv[]) {
	unsigned int target = 100000;

	// Check for CLI parameter: target
	if (argc > 1) {
		char *pCh;
		target = (unsigned int) strtoul(argv[1], &pCh, 10);
	}

	// Start timer
	struct timeval tmStart, tmEnd;
	gettimeofday(&tmStart, NULL);

	vector_uint* numbers = compute(target);

	// Stop timer
	gettimeofday(&tmEnd, NULL);
	unsigned long long duration = 1000 * (
		tmEnd.tv_sec - tmStart.tv_sec
	) + (
		tmEnd.tv_usec - tmStart.tv_usec
	) / 1000;
	
	// Print results
	printf("target:        %u\n", target);
	printf("prime numbers: %u\n", numbers->_width);
	printf("time:          %llu ms\n", duration);
}