# This source code is released into the public domain.

add_library(nihil.cli STATIC)
target_link_libraries(nihil.cli PRIVATE nihil.util)
target_sources(nihil.cli
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.cli.ccm

	command.ccm
	command_tree.ccm
	command_node.ccm
	dispatch_command.ccm
	registry.ccm
	usage_error.ccm

	PRIVATE
	command.cc
	command_tree.cc
	command_node.cc
	dispatch_command.cc
	registry.cc
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.cli.test
		test.cc
	)
	target_link_libraries(nihil.cli.test PRIVATE
		nihil.cli
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.cli.test)
endif()
