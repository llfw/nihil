# This source code is released into the public domain.

cmake_minimum_required(VERSION 3.28)

project(nihil)

option(NIHIL_CONFIG	"Build the nihil.config library"	ON)
option(NIHIL_UCL	"Build the nihil.ucl library"		ON)
option(NIHIL_TESTS	"Build nihil's unit tests"		ON)

set(CMAKE_CXX_STANDARD 26)

find_package(PkgConfig REQUIRED)

add_compile_options(-W)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wpedantic)

add_subdirectory(nihil.flagset)
add_subdirectory(nihil.uuid)
add_subdirectory(nihil)

if(NIHIL_UCL)
	add_subdirectory(nihil.ucl)
endif()

if(NIHIL_CONFIG)
	add_subdirectory(nihil.config)
endif()

if(NIHIL_TESTS)
	enable_testing()
endif()
