project(sdizo_project1 CXX)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
if(UNIX)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb -fsanitize=address")
endif(UNIX)
project(sdizo CXX)

if(MSVC)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
find_package(cppfs CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
endif(MSVC)

file(GLOB_RECURSE CXX_SOURCE src/*.cc)
add_executable(${PROJECT_NAME} ${CXX_SOURCE})

if(UNIX)
target_link_libraries(${PROJECT_NAME} stdc++fs fmt)
endif(UNIX)

if(MSVC)
target_link_libraries(${PROJECT_NAME} cppfs::cppfs fmt::fmt fmt::fmt-header-only)
endif(MSVC)

target_compile_options(${PROJECT_NAME} PRIVATE /MT)

