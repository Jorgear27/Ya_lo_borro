#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memory.h"
#include "unity.h"

#define NUM_ALLOCATIONS 5000
#define MAX_ALLOCATION_SIZE 1024

void setUp(void) {
    // Set up code if needed
}

void tearDown(void) {
    // Tear down code if needed
}

double calculate_fragmentation(size_t total_free, size_t total_memory, int num_free_blocks, int total_blocks) {
    double block_fragmentation = (double)num_free_blocks / total_blocks * 100;
    double space_fragmentation = (double)total_free / total_memory * 100;
    return block_fragmentation * 0.5 + space_fragmentation * 0.5;
}

void test_policy(int policy, const char* policy_name) {
    malloc_control(policy);

    void* allocations[NUM_ALLOCATIONS];
    size_t allocated = 0, total_free = 0, total_memory = 0;
    int num_free_blocks = 0, total_blocks = 0;
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        size_t size = rand() % MAX_ALLOCATION_SIZE + 1;
        allocations[i] = malloc(size);
        if (allocations[i]) {
            allocated += size;
        }
    }

    for (int i = 0; i < NUM_ALLOCATIONS / 2; i++) {
        int index = rand() % NUM_ALLOCATIONS;
        if (allocations[index]) {
            free(allocations[index]);
            allocations[index] = NULL;
        }
    }

    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        if (allocations[i] == NULL) {
            size_t size = rand() % MAX_ALLOCATION_SIZE + 1;
            allocations[i] = malloc(size);
            if (allocations[i]) {
                allocated += size;
            }
        }
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    memory_usage(&allocated, &total_free);

    // Calculate the number of free blocks and total blocks
    t_block b = base;
    while (b) {
        total_blocks++;
        if (b->free) {
            num_free_blocks++;
            total_free += b->size;
        }
        total_memory += b->size;
        b = b->next;
    }

    double fragmentation = calculate_fragmentation(total_free, total_memory, num_free_blocks, total_blocks);

    printf("%s - TIME: %f seconds, ALLOCATED: %zu bytes, FREE: %zu bytes, FRAGMENTATION: %f\n\n", policy_name, cpu_time_used, allocated, total_free, fragmentation);

    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        if (allocations[i]) {
            free(allocations[i]);
        }
    }
}

void test_policies() {
    printf("Testing First Fit...\n");
    test_policy(FIRST_FIT, "First Fit");

    printf("Testing Best Fit...\n");
    test_policy(BEST_FIT, "Best Fit");

    printf("Testing Worst Fit...\n");
    test_policy(WORST_FIT, "Worst Fit");
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_policies);
    printf("\nMemory Policies Test Completed:\n");
    printf("    Check the log file for more details\n\n");
    return UNITY_END();
}
