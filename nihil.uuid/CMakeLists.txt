# This source code is released into the public domain.

add_library(nihil.uuid STATIC)
target_sources(nihil.uuid
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	uuid.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.uuid.test test.cc)
	target_link_libraries(nihil.uuid.test PRIVATE
		nihil.uuid
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.uuid.test)
endif()
