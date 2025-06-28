# This source code is released into the public domain.

add_library(nihil.cli STATIC)
target_link_libraries(nihil.cli PRIVATE nihil.util)
target_sources(nihil.cli
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.cli.ccm

	command_map.ccm
	usage_error.ccm

	PRIVATE
	command_map.cc
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.cli.test
		test_command_map.cc
	)
	target_link_libraries(nihil.cli.test PRIVATE
		nihil.cli
		Catch2::Catch2WithMain
	)

	find_package(Catch2 REQUIRED)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.cli.test)
endif()
