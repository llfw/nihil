# This source code is released into the public domain.

cmake_minimum_required(VERSION 3.28)

project(nihil)

option(NIHIL_CONFIG	"Build the nihil.config library"	ON)
option(NIHIL_UCL	"Build the nihil.ucl library"		ON)
option(NIHIL_TESTS	"Build nihil's unit tests"		ON)
option(NIHIL_TIDY	"Run clang-tidy during build"		ON)

set(CMAKE_CXX_STANDARD 26)

find_package(PkgConfig REQUIRED)

# clang-tidy support
find_program(CLANG_TIDY clang-tidy)

if(NOT (CLANG_TIDY STREQUAL ""))
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

	file(GLOB_RECURSE NIHIL_SOURCES "*.cc" "*.ccm")
	list(FILTER NIHIL_SOURCES EXCLUDE REGEX ${CMAKE_CURRENT_BINARY_DIR})

	add_custom_target(tidy COMMAND
		${CLANG_TIDY}	-config-file=${CMAKE_CURRENT_SOURCE_DIR}/clang-tidy.conf
				-p=${CMAKE_CURRENT_BINARY_DIR}
				${NIHIL_SOURCES})
	
	if(NIHIL_TIDY)
		set(CMAKE_CXX_CLANG_TIDY
			${CLANG_TIDY};
			-config-file=${CMAKE_CURRENT_SOURCE_DIR}/clang-tidy.conf;
		)
	endif()
endif()

add_compile_options(-W)
add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)
add_compile_options(-Wpedantic)

add_subdirectory(nihil.cli)
add_subdirectory(nihil.core)
add_subdirectory(nihil.error)
add_subdirectory(nihil.flagset)
add_subdirectory(nihil.generator)
add_subdirectory(nihil.guard)
add_subdirectory(nihil.match)
add_subdirectory(nihil.monad)
add_subdirectory(nihil.posix)
add_subdirectory(nihil.util)
add_subdirectory(nihil.uuid)

if(NIHIL_UCL)
	add_subdirectory(nihil.ucl)
endif()

if(NIHIL_CONFIG)
	add_subdirectory(nihil.config)
endif()

if(NIHIL_TESTS)
	enable_testing()
endif()
