# This source code is released into the public domain.

add_library(nihil.error STATIC)
target_link_libraries(nihil.error PRIVATE nihil.std nihil.match)
target_sources(nihil.error
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	error.ccm
	sys_error.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.error.test
		test.cc)

	target_link_libraries(nihil.error.test PRIVATE
		nihil.error
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.error.test)
endif()
