# This source code is released into the public domain.

add_library(nihil.util STATIC)
target_link_libraries(nihil.util PRIVATE
	nihil.std
	nihil.core
	nihil.error
	nihil.monad
)
target_sources(nihil.util
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.util.ccm

	capture_stream.ccm
	construct.ccm
	ctype.ccm
	guard.ccm
	parse_size.ccm
	next_word.ccm
	save_errno.ccm
	skipws.ccm
	tabulate.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.util.test
		capture_stream.test.cc
		ctype.test.cc
		guard.test.cc
		parse_size.test.cc
		next_word.test.cc
		skipws.test.cc
		tabulate.test.cc
	)
	target_link_libraries(nihil.util.test PRIVATE
		nihil.util
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.util.test)
endif()
