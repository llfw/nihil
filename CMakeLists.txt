# This source code is released into the public domain.

cmake_minimum_required(VERSION 3.28)

project(libexi)

set(CMAKE_CXX_STANDARD 26)

add_compile_options(-W)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wpedantic)

add_subdirectory(modules)
add_subdirectory(tests)

enable_testing()
