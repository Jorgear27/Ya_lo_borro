# Version check
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Set the project name
project(my_memory VERSION 1.0 DESCRIPTION "My own memory library" LANGUAGES C)

# Add the library
add_library(${PROJECT_NAME} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/memory.c)

# Set the library properties
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Enable testing
include(CTest)
enable_testing()

# Add the tests
add_subdirectory(tests)

# Add the tests
add_test(NAME "MemoryLibrary_tests" COMMAND test_memory WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lib/memory)
add_test(NAME "Policies_tests" COMMAND test_policies WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lib/memory)

# Add the subdirectory for the app
add_subdirectory(app)
