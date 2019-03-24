project(sdizo_project1 CXX)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb -fsanitize=address")
set(OUTPUT_NAME sdizo)

file(GLOB_RECURSE CXX_SOURCE src/*.cc)

add_executable(${OUTPUT_NAME} ${CXX_SOURCE})
