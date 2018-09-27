#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

typedef enum { false, true } bool;

/* NUMBER VECTOR */

// vector_uint represents a vector of unsigned integers.
//
// use new_vector_uint() to create a new instance.
// use vector_uint_pushback() to append a new value
typedef struct {
	pthread_mutex_t mutex;
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

/* CALCULATOR */

typedef struct {
	unsigned int _target;          // Calculation target
	unsigned int _progress;        // Current progress
	pthread_mutex_t _lock_state;   // State lock
	size_t _thread_pool_width;     // Number of used threads
	pthread_t** _thread_pool;      // Worker threads
	pthread_mutex_t _lock_numbers; // Numbers vector lock
	vector_uint* _numbers;         // Calculated numbers
} c_prime_calc;

// calculator_worker is executed by a worker thread
void* calculator_worker(void* c) {
	c_prime_calc* calc = (c_prime_calc*) c;
	while (1) {
		// Synchronize access to _progress
		pthread_mutex_lock(&calc->_lock_state);
		++calc->_progress;
		const unsigned int current_number = calc->_progress;
		pthread_mutex_unlock(&calc->_lock_state);

		// Exit loop when the target was reached
		if (current_number > calc->_target) {
			break;
		}

		// Check whether the current number is a prime number
		if (is_prime_sqrt(current_number)) {
			// Synchronize access to the numbers vector
			pthread_mutex_lock(&calc->_lock_numbers);
			vector_uint_pushback(calc->_numbers, current_number);
			pthread_mutex_unlock(&calc->_lock_numbers);
		}
	}
}

// new_c_prime_calc creates a new concurrent prime number
// calculator instance
c_prime_calc* new_c_prime_calc(
	const unsigned int target,
	const size_t threads_num
) {
	// Allocate an instance on the heap
	c_prime_calc* calc = (c_prime_calc*) malloc(
		sizeof(c_prime_calc)
	);
	if (!calc) {
		return NULL; // Allocation failure
	}

	// Initialize fields
	calc->_target = target;
	calc->_progress = 0;
	calc->_numbers = new_vector_uint(10);

	// Initialize state lock
	pthread_mutexattr_t sharedLockState;
	pthread_mutexattr_init(&sharedLockState);
	pthread_mutexattr_setpshared(&sharedLockState, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&(calc->_lock_state), &sharedLockState);

	// Initialize numbers lock
	pthread_mutexattr_t sharedLockNumbers;
	pthread_mutexattr_init(&sharedLockNumbers);
	pthread_mutexattr_setpshared(&sharedLockNumbers, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&(calc->_lock_numbers), &sharedLockNumbers);

	// Initialize thread pool
	calc->_thread_pool_width = threads_num;
	calc->_thread_pool = (pthread_t**) malloc(sizeof(pthread_t*) * threads_num);
	for (size_t i = 0; i < threads_num; ++i) {
		calc->_thread_pool[i] = (pthread_t*) malloc(sizeof(pthread_t));
	}

	return calc;
}

// c_prime_calc_run starts the calculation and blocks until all threads have
// finished execution
void c_prime_calc_run(c_prime_calc* calc) {
	size_t thread_num = calc->_thread_pool_width;
	for (size_t i = 0; i < thread_num; ++i) {
		// Initialize individual threads
		int err = pthread_create(
			calc->_thread_pool[i], // Thread
			NULL,                  // Attributes
			calculator_worker,     // Start routine
			(void*) calc           // Arguments
		);
		if (err) {
			fprintf(stderr,"Error - pthread_create() return code: %d\n", err);
			exit(EXIT_FAILURE);
		}
	}

	// Block until all threads have finished execution
	for (size_t i = 0; i < thread_num; ++i) {
		pthread_join(*calc->_thread_pool[i], NULL);
	}
}

void main(int argc, char *argv[]) {
	unsigned int target = 100000; // Default target
	size_t parallel_workers = 1;  // Number of threads

	// Read CLI parameter: target
	if (argc > 1) {
		char *pCh;
		target = (unsigned int) strtoul(argv[1], &pCh, 10);
	}

	// Read CLI parameter: parallel workers
	if (argc > 2) {
		char *pCh;
		parallel_workers = (size_t) strtoul(argv[2], &pCh, 10);
	}

	// Initialize the concurrent prime number calculator
	c_prime_calc* calc = new_c_prime_calc(target, parallel_workers);

	// Start timer
	struct timeval tmStart, tmEnd;
	gettimeofday(&tmStart, NULL);

	// Begin calculation
	c_prime_calc_run(calc);

	// Stop timer
	gettimeofday(&tmEnd, NULL);
	unsigned long long duration = 1000 * (
		tmEnd.tv_sec - tmStart.tv_sec
	) + (
		tmEnd.tv_usec - tmStart.tv_usec
	) / 1000;
		
	
	// Print results
	printf("target:        %u\n", target);
	printf("prime numbers: %u\n", calc->_numbers->_width);
	printf("time:          %llu ms\n", duration);
}