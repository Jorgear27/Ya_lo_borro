# Verify cmake version
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Create project
project (MemoryProject VERSION 1.0 DESCRIPTION "Memory Project" LANGUAGES C)

# Find dependencies in CONAN
find_package(unity REQUIRED)
find_package(cJSON REQUIRED)

# Flags for compiling
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wpedantic -Werror -Wextra -Wunused-parameter -Wmissing-prototypes -Wstrict-prototypes")

# Include directories
include_directories(include)
include_directories(lib/memory/include)

# Añadir los archivos fuente
set(SOURCES
    src/main.c
    src/commands.c
    src/utils.c
    src/monitor_commands.c
)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the libraries
target_link_libraries(${PROJECT_NAME} PRIVATE cjson::cjson unity::unity)

# Add the memory library
add_subdirectory(lib/memory)

# Add Metrics Monitoring System
add_subdirectory(Metrics_Monitoring_System)

# Enable testing
if(RUN_COVERAGE)
    message("Coverage enabled")
    add_subdirectory(tests)
endif()
