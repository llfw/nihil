# This source code is released into the public domain.

add_library(nihil.config STATIC)
target_link_libraries(nihil.config PRIVATE
	nihil.error
	nihil.generator
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
	read.cc
	store.cc
	string.cc
	write.cc
)

if(NIHIL_TESTS)
	add_subdirectory(tests)
	enable_testing()
endif()
