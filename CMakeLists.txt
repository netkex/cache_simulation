cmake_minimum_required(VERSION 3.17)
project(cache_sim)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O2)

include_directories(include)

add_executable(cache_sim src/main.cpp src/f.cpp src/Memory.cpp)