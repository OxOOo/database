project(database_exp1)
cmake_minimum_required(VERSION 3.5)

add_compile_options(--std=c++11)
add_compile_options(-Wall)
add_compile_options(-O3)
add_compile_options(-g)
add_compile_options(-MMD -MP)

file(GLOB SOURCES src/*.cpp)
add_executable(main ${SOURCES})