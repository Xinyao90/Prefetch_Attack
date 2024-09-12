#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <x86intrin.h>

// Define the size of memory to allocate.
#define ALLOCATION_SIZE (2 * 1024 * 1024) // 2MB size.
#define NUM_ACCESSES 1000000

// Function to perform memory access with software prefetching.
void memory_access_with_prefetch(volatile char *buffer, size_t size) {
    size_t step = 64; // Cache line size.
    for (size_t i = 0; i < size; i += step) {
        _mm_prefetch(&buffer[i], _MM_HINT_T0);  // Use PREFETCHT0 for L1 cache.
        buffer[i] = (char)(i % 256);            // Access the memory.
    }
}

// Function to measure access time for the buffer.
double measure_access_time(volatile char *buffer, size_t size) {
    uint64_t start, end;
    double total_time = 0.0;

    for (int i = 0; i < NUM_ACCESSES; i++) {
        _mm_prefetch(&buffer[(i * 64) % size], _MM_HINT_NTA);  // Use PREFETCHNTA for non-temporal data.
        start = __rdtsc(); // Read Time-Stamp Counter.
        buffer[(i * 64) % size]++; // Simulate memory access.
        end = __rdtsc();
        total_time += (end - start);
    }

    return total_time / NUM_ACCESSES;
}

int main() {
    volatile char *buffer;

    // Allocate regular memory using malloc.
    buffer = (char *)malloc(ALLOCATION_SIZE);
    if (!buffer) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Perform memory access pattern with prefetching.
    memory_access_with_prefetch(buffer, ALLOCATION_SIZE);

    // Measure access time and print the results.
    double access_time = measure_access_time(buffer, ALLOCATION_SIZE);
    printf("Average memory access time without huge pages and with prefetching: %lf cycles\n", access_time);

    // Clean up and release resources.
    free((void *)buffer);
    return 0;
}

