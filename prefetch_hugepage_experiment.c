#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <x86intrin.h>

// Define the size of memory to allocate.
#define ALLOCATION_SIZE (2 * 1024 * 1024) // 2MB huge page size.
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
    int fd;
    volatile char *buffer;

    // Open the huge page file.
    fd = open("/mnt/hugepages/hugepagefile", O_CREAT | O_RDWR, 0755);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Allocate memory using mmap with huge page support.
    buffer = (char *)mmap(NULL, ALLOCATION_SIZE, PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_HUGETLB, fd, 0);
    if (buffer == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Perform memory access pattern with prefetching.
    memory_access_with_prefetch(buffer, ALLOCATION_SIZE);

    // Measure access time and print the results.
    double access_time = measure_access_time(buffer, ALLOCATION_SIZE);
    printf("Average memory access time with huge pages and prefetching: %lf cycles\n", access_time);

    // Clean up and release resources.
    munmap((void *)buffer, ALLOCATION_SIZE);
    close(fd);
    return 0;
}

