# This source code is released into the public domain.

pkg_check_modules(LIBUCL REQUIRED libucl)

add_library(nihil.ucl STATIC)
target_link_libraries(nihil.ucl PUBLIC nihil)
target_sources(nihil.ucl PUBLIC
	FILE_SET modules TYPE CXX_MODULES FILES
	nihil.ucl.ccm
	emit.ccm
	error.ccm
	object.ccm
	parser.ccm

	array.ccm
	boolean.ccm
	integer.ccm
	real.ccm
	string.ccm
)

target_compile_options(nihil.ucl PUBLIC ${LIBUCL_CFLAGS_OTHER})
target_include_directories(nihil.ucl PUBLIC ${LIBUCL_INCLUDE_DIRS})
target_link_libraries(nihil.ucl PUBLIC ${LIBUCL_LIBRARIES})
target_link_directories(nihil.ucl PUBLIC ${LIBUCL_LIBRARY_DIRS})

if(NIHIL_TESTS)
	add_subdirectory(tests)
	enable_testing()
endif()
