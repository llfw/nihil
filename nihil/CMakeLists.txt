# This source code is released into the public domain.

add_library(nihil STATIC)
target_sources(nihil PUBLIC
	FILE_SET modules TYPE CXX_MODULES FILES
	nihil.ccm
	command_map.ccm
	ctype.ccm
	fd.ccm
	generator.ccm
	generic_error.ccm
	getenv.ccm
	guard.ccm
	next_word.ccm
	skipws.ccm
	tabulate.ccm
	usage_error.ccm)
