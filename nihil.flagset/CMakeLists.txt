# This source code is released into the public domain.

add_library(nihil.flagset STATIC)
target_sources(nihil.flagset
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	flagset.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.flagset.test test.cc)
	target_link_libraries(nihil.flagset.test PRIVATE
		nihil.flagset
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.flagset.test)
endif()
