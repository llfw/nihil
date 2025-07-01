# This source code is released into the public domain.

add_library(nihil.std STATIC)
target_sources(nihil.std
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.std.ccm
)
