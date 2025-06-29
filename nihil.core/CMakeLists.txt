# This source code is released into the public domain.

add_library(nihil.core STATIC)
target_include_directories(nihil.core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(nihil.core
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.core.ccm
	errc.ccm

	PRIVATE
	errc.cc
)
