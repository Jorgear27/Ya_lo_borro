/**
 * @file policies_stats.h
 * @author Jorge Arbach
 * @brief 
 * @version 0.1
 * @date 2024-12-04
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "memory.h"
#include <string.h>
#include <cjson/cJSON.h>

/**
 * @brief Number of allocations to be made
 * 
 */ 
#define NUM_ALLOCATIONS 500

/**
 * @brief Maximum size of an allocation
 * 
 */
#define MAX_ALLOCATION_SIZE 1024

/**
 * @brief File path to store the policy statistics
 * 
 */
#define JSON_PATH getenv("JSON_PATH")

/**
 * @brief Structure to store the statistics of a policy
 * 
 */
typedef struct {
    double time; /**< Time taken to allocate and free memory */
    double fragmentation; /**< Fragmentation percentage */
} PolicyStats;

/**
 * @brief Calculate the fragmentation of the memory
 *
 * @param total_free Total free memory
 * @param total_memory Total memory
 * @param num_free_blocks Number of free blocks
 * @param total_blocks Total blocks
 * @return double Fragmentation percentage
 */
double calculate_fragmentation(size_t total_free, size_t total_memory, int num_free_blocks, int total_blocks);

/**
 * @brief Test a memory allocation policy
 *
 * @param policy Policy to test
 * @param policy_name Name of the policy
 */
PolicyStats test_policy(int policy, const char* policy_name);
