# This source code is released into the public domain.

cmake_minimum_required(VERSION 3.28)

project(nihil)

option(NIHIL_CONFIG	"Build the nihil.config library"	ON)
option(NIHIL_UCL	"Build the nihil.ucl library"		ON)
option(NIHIL_TESTS	"Build nihil's unit tests"		ON)
option(NIHIL_TIDY	"Run clang-tidy during build"		ON)
option(NIHIL_ASAN	"Enable Address Sanitizer"		OFF)
option(NIHIL_UBSAN	"Enable Undefined Behaviour Sanitizer"	OFF)

set(CMAKE_CXX_STANDARD 26)

find_package(PkgConfig REQUIRED)

# clang-tidy support
find_program(CLANG_TIDY clang-tidy)

if (NOT (CLANG_TIDY STREQUAL "CLANG_TIDY-NOTFOUND"))
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

if(NIHIL_ASAN)
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()

if(NIHIL_UBSAN)
	add_compile_options(-fsanitize=undefined)
	add_link_options(-fsanitize=undefined)
endif()

# Enable libc++ hardening
add_compile_definitions(-D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE)

if(NIHIL_TESTS)
	add_subdirectory(contrib/catch2)
	enable_testing()
endif()

add_subdirectory(nihil.std)
add_subdirectory(nihil.cli)
add_subdirectory(nihil.core)
add_subdirectory(nihil.generator)
add_subdirectory(nihil.posix)
add_subdirectory(nihil.util)

if(NIHIL_UCL)
	add_subdirectory(nihil.ucl)
endif()

if(NIHIL_CONFIG)
	add_subdirectory(nihil.config)
endif()
