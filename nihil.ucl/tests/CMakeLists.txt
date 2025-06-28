# This source code is released into the public domain.

add_executable(nihil.ucl.test
	emit.cc
	parse.cc

	object.cc
	array.cc
	boolean.cc
	integer.cc
	map.cc
	real.cc
	string.cc
)

target_link_libraries(nihil.ucl.test PRIVATE nihil.ucl Catch2::Catch2WithMain)

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)
catch_discover_tests(nihil.ucl.test)
