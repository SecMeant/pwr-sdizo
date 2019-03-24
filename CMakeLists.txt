project(sdizo_project1 CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra ")
set(OUTPUT_NAME sdizo)

file(GLOB_RECURSE CXX_SOURCE src/*.cc)

add_executable(${OUTPUT_NAME} ${CXX_SOURCE})
