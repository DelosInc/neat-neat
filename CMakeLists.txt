cmake_minimum_required(VERSION 3.1)

project(neat-neat)

include_directories(include/neat-neat)

file(GLOB neat_src src/*)
add_library(neat ${neat_src})

add_executable(main src/main.cpp)
target_compile_options(main PUBLIC -std=c++2a)
target_link_libraries(main neat)