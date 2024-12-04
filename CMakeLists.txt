# Verify cmake version
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Create project
project (MemoryProject VERSION 1.0 DESCRIPTION "Memory Project" LANGUAGES C)

# Find dependencies in CONAN
find_package(unity REQUIRED)
find_package(cJSON REQUIRED)

# Add the Memory directory
add_subdirectory(lib/memory)

add_subdirectory(My_Shell)

add_subdirectory(My_Shell/Metrics_Monitoring_System)
