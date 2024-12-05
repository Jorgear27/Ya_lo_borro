#include "memory.h"
#include "unity.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void setUp(void)
{
    // Set up code if needed
}

void tearDown(void)
{
    // Tear down code if needed
}

void test_malloc()
{
    printf("Testing malloc...\n");
    void* ptr = malloc(100);
    TEST_ASSERT_NOT_NULL(ptr);
    printf("Memory allocated at: %p\n", ptr);
    free(ptr);
    printf("Memory freed at: %p\n\n", ptr);
}

void test_calloc()
{
    printf("Testing calloc...\n");
    void* ptr = calloc(10, sizeof(int));
    TEST_ASSERT_NOT_NULL(ptr);
    printf("Memory allocated and zeroed at: %p\n", ptr);
    for (int i = 0; i < 10; i++)
    {
        TEST_ASSERT_EQUAL_INT(0, ((int*)ptr)[i]);
    }
    free(ptr);
    printf("Memory freed at: %p\n\n", ptr);
}

void test_realloc()
{
    printf("Testing realloc...\n");
    void* ptr = malloc(50);
    TEST_ASSERT_NOT_NULL(ptr);
    printf("Memory allocated at: %p\n", ptr);
    ptr = realloc(ptr, 100);
    TEST_ASSERT_NOT_NULL(ptr);
    printf("Memory reallocated at: %p\n", ptr);
    free(ptr);
    printf("Memory freed at: %p\n\n", ptr);
}

void test_free()
{
    printf("Testing free...\n");
    void* ptr = malloc(100);
    TEST_ASSERT_NOT_NULL(ptr);
    printf("Memory allocated at: %p\n", ptr);
    free(ptr);
    printf("Memory freed at: %p\n\n", ptr);
}

void test_memory_usage()
{
    printf("Testing memory usage...\n");
    size_t allocated, free_space;
    memory_usage(&allocated, &free_space);
    printf("Memory allocated: %zu bytes\n", allocated);
    printf("Memory free: %zu bytes\n\n", free_space);
    TEST_ASSERT_GREATER_THAN(0, allocated + free_space);
}

void test_malloc_and_free()
{
    printf("Testing malloc and free...\n");
    void* ptr1 = malloc(100);
    void* ptr2 = malloc(200);
    TEST_ASSERT_NOT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
    printf("Memory allocated at: %p and %p\n", ptr1, ptr2);
    free(ptr1);
    free(ptr2);
    printf("Memory freed at: %p and %p\n\n", ptr1, ptr2);
}

void test_calloc_and_free()
{
    printf("Testing calloc and free...\n");
    void* ptr1 = calloc(10, sizeof(int));
    void* ptr2 = calloc(20, sizeof(int));
    TEST_ASSERT_NOT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
    printf("Memory allocated and zeroed at: %p and %p\n", ptr1, ptr2);
    free(ptr1);
    free(ptr2);
    printf("Memory freed at: %p and %p\n\n", ptr1, ptr2);
}

void test_realloc_and_free()
{
    printf("Testing realloc and free...\n");
    void* ptr1 = malloc(50);
    void* ptr2 = malloc(100);
    TEST_ASSERT_NOT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
    printf("Memory allocated at: %p and %p\n", ptr1, ptr2);
    ptr1 = realloc(ptr1, 150);
    ptr2 = realloc(ptr2, 200);
    TEST_ASSERT_NOT_NULL(ptr1);
    TEST_ASSERT_NOT_NULL(ptr2);
    printf("Memory reallocated at: %p and %p\n", ptr1, ptr2);
    free(ptr1);
    free(ptr2);
    printf("Memory freed at: %p and %p\n\n", ptr1, ptr2);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_malloc);
    RUN_TEST(test_calloc);
    RUN_TEST(test_realloc);
    RUN_TEST(test_free);
    RUN_TEST(test_memory_usage);
    RUN_TEST(test_malloc_and_free);
    RUN_TEST(test_calloc_and_free);
    RUN_TEST(test_realloc_and_free);
    printf("All tests passed!\n");
    return UNITY_END();
}
