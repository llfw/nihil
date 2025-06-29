# This source code is released into the public domain.

add_library(nihil.guard STATIC)
target_sources(nihil.guard
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	guard.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.guard.test test.cc)
	target_link_libraries(nihil.guard.test PRIVATE
		nihil.guard
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.guard.test)
endif()
