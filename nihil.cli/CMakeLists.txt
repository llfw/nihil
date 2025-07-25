# This source code is released into the public domain.

add_library(nihil.cli STATIC)

target_link_libraries(nihil.cli PRIVATE
	nihil.std
	nihil.core
	nihil.posix
)

target_sources(nihil.cli
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.cli.ccm

	command.ccm
	command_tree.ccm
	dispatch_command.ccm
	parse_flags.ccm
	registry.ccm
	usage_error.ccm

	PRIVATE
	dispatch_command.cc
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.cli.test
		command.test.cc
		command_tree.test.cc
		dispatch_command.test.cc
		parse_flags.test.cc
	)

	target_link_libraries(nihil.cli.test PRIVATE
		nihil.std
		nihil.core
		nihil.cli
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.cli.test)
endif()
