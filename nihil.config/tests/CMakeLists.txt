# This source code is released into the public domain.

add_executable(nihil.config.test
	string.cc
)

target_link_libraries(nihil.config.test PRIVATE
	nihil.config
	Catch2::Catch2WithMain)

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)
catch_discover_tests(nihil.config.test)
