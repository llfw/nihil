# This source code is released into the public domain.

add_library(nihil STATIC)
target_sources(nihil PUBLIC
	FILE_SET modules TYPE CXX_MODULES FILES
	nihil.ccm
	argv.ccm
	command_map.ccm
	ctype.ccm
	exec.ccm
	fd.ccm
	find_in_path.ccm
	generator.ccm
	generic_error.ccm
	getenv.ccm
	guard.ccm
	next_word.ccm
	process.ccm
	skipws.ccm
	spawn.ccm
	tabulate.ccm
	usage_error.ccm)
