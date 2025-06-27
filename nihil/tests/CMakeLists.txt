# This source code is released into the public domain.

add_executable(nihil.test
	command_map.cc
	ctype.cc
	error.cc
	fd.cc
	generator.cc
	getenv.cc
	guard.cc
	monad.cc
	next_word.cc
	skipws.cc
	spawn.cc
	tabulate.cc
	uuid.cc
)

target_link_libraries(nihil.test PRIVATE
	nihil
	Catch2::Catch2WithMain
)

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)
catch_discover_tests(nihil.test)
