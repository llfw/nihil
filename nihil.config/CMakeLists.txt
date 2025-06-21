# This source code is released into the public domain.

add_library(nihil.config STATIC)
target_sources(nihil.config PUBLIC
	FILE_SET modules TYPE CXX_MODULES FILES
	nihil.config.ccm
	error.ccm
	store.ccm
	option.ccm
	string.ccm)
target_link_libraries(nihil.config PUBLIC nihil nihil.ucl)
