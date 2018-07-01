cmake_minimum_required (VERSION 2.8.12)

project (benchmark)

# Ensure we have pthreads
find_package(Threads REQUIRED)

#Enable C++ 11 standard
set(CMAKE_CXX_STANDARD 11)

# Set up directories
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(runbench bench.cc)

target_link_libraries(runbench benchmark pthread)
