# This source code is released into the public domain.

add_executable(nihil.test
	command_map.cc
	ctype.cc
	fd.cc
	generator.cc
	generic_error.cc
	getenv.cc
	guard.cc
	next_word.cc
	skipws.cc
	spawn.cc
	tabulate.cc)

target_link_libraries(nihil.test PRIVATE
	nihil
	Catch2::Catch2WithMain
)

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)
catch_discover_tests(nihil.test)
