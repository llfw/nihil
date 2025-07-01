# This source code is released into the public domain.

pkg_check_modules(LIBUCL REQUIRED libucl)

add_library(nihil.ucl STATIC)
target_link_libraries(nihil.ucl PRIVATE nihil.std nihil.core nihil.error nihil.monad)

target_sources(nihil.ucl
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.ucl.ccm
	emit.ccm
	object.ccm
	object_cast.ccm
	parser.ccm
	type.ccm

	array.ccm
	boolean.ccm
	integer.ccm
	map.ccm
	real.ccm
	string.ccm
)

target_compile_options(nihil.ucl PUBLIC ${LIBUCL_CFLAGS_OTHER})
target_include_directories(nihil.ucl PUBLIC ${LIBUCL_INCLUDE_DIRS})
target_link_libraries(nihil.ucl PUBLIC ${LIBUCL_LIBRARIES})
target_link_directories(nihil.ucl PUBLIC ${LIBUCL_LIBRARY_DIRS})

if(NIHIL_TESTS)
	add_executable(nihil.ucl.test
		array.test.cc
		boolean.test.cc
		emit.test.cc
		integer.test.cc
		map.test.cc
		object.test.cc
		parse.test.cc
		real.test.cc
		string.test.cc
	)

	target_link_libraries(nihil.ucl.test PRIVATE nihil.ucl Catch2::Catch2WithMain)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.ucl.test)

	enable_testing()
endif()
