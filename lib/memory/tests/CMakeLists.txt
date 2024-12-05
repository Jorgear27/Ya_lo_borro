# Verify cmake version
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Set compiler and linker flags for coverage
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")

# Create the executable for the tests
add_executable(test_memory test_memory.c ../src/memory.c)
add_executable(test_policies test_policies.c ../src/memory.c)

# Link the libraries
target_link_libraries(test_memory PRIVATE my_memory unity::unity gcov)
target_link_libraries(test_policies PRIVATE my_memory unity::unity gcov)

# Set the output directory
set_target_properties(test_memory PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/memory/tests)
set_target_properties(test_policies PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/memory/tests)
