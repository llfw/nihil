# This source code is released into the public domain.

add_library(nihil.util STATIC)
target_link_libraries(nihil.util PRIVATE nihil.core nihil.error nihil.monad)
target_sources(nihil.util
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.util.ccm

	capture_stream.ccm
	ctype.ccm
	parse_size.ccm
	next_word.ccm
	skipws.ccm
	tabulate.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.util.test
		test_capture_stream.cc
		test_ctype.cc
		test_parse_size.cc
		test_next_word.cc
		test_skipws.cc
		test_tabulate.cc
	)
	target_link_libraries(nihil.util.test PRIVATE
		nihil.util
		Catch2::Catch2WithMain
	)

	find_package(Catch2 REQUIRED)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.util.test)
endif()
