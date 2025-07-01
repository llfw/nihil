# This source code is released into the public domain.

add_library(nihil.monad STATIC)
target_link_libraries(nihil.monad PRIVATE nihil.std nihil.error)
target_sources(nihil.monad
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	monad.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.monad.test test.cc)
	target_link_libraries(nihil.monad.test PRIVATE
		nihil.monad
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.monad.test)
endif()
