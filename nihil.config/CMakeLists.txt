# This source code is released into the public domain.

add_library(nihil.config STATIC)

target_link_libraries(nihil.config PRIVATE
	nihil.std
	nihil.core
	nihil.posix
	nihil.ucl
)

target_sources(nihil.config
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.config.ccm
	read.ccm
	store.ccm
	write.ccm

	option.ccm
	string.ccm

	PRIVATE
	option.cc
	store.cc
)

if(NIHIL_TESTS)
	add_executable(nihil.config.test
		string.test.cc
	)

	target_link_libraries(nihil.config.test PRIVATE
		nihil.std
		nihil.core
		nihil.config
		Catch2::Catch2WithMain)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.config.test)

	enable_testing()
endif()
