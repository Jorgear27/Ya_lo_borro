# Version check
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Set the project name
project(policies_stats VERSION 1.0 DESCRIPTION "Updates policies stats regularly" LANGUAGES C)

# Flags for compiling
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wpedantic -Werror -Wextra -Wunused-parameter -Wmissing-prototypes -Wstrict-prototypes")

# Find dependencies en Conan
find_package(cJSON REQUIRED)

# Include directories
include_directories(include)

# Create the executable for the program
add_executable(${PROJECT_NAME} src/policies_stats.c)

# Link the libraries
target_link_libraries(policies_stats PRIVATE cjson::cjson my_memory)

# Set the output directory
set_target_properties(policies_stats PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/memory)
