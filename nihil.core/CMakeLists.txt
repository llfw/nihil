# This source code is released into the public domain.

add_library(nihil.core STATIC)
target_link_libraries(nihil.core PRIVATE nihil.std)
target_include_directories(nihil.core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(nihil.core
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.core.ccm

	capture_stream.ccm
	construct.ccm
	ctype.ccm
	errc.ccm
	error.ccm
	features.ccm
	flagset.ccm
	generator.ccm
	guard.ccm
	match.ccm
	monad.ccm
	next_word.ccm
	parse_size.ccm
	save_errno.ccm
	skipws.ccm
	sys_error.ccm
	tabulate.ccm
	uuid.ccm
)

if (NIHIL_TESTS)
	add_executable(nihil.core.test
		capture_stream.test.cc
		ctype.test.cc
		error.test.cc
		flagset.test.cc
		generator.test.cc
		guard.test.cc
		match.test.cc
		monad.test.cc
		next_word.test.cc
		parse_size.test.cc
		skipws.test.cc
		tabulate.test.cc
	)

	target_link_libraries(nihil.core.test PRIVATE
		nihil.std
		nihil.core
		Catch2::Catch2WithMain)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.core.test)

	enable_testing()
endif ()
