# This source code is released into the public domain.

add_library(nihil.match STATIC)
target_sources(nihil.match
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	match.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.match.test test.cc)
	target_link_libraries(nihil.match.test PRIVATE
		nihil.match
		Catch2::Catch2WithMain
	)

	find_package(Catch2 REQUIRED)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.match.test)
endif()
