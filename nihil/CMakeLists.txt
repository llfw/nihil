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
	format_filesystem.ccm
	generator.ccm
	generic_error.ccm
	getenv.ccm
	guard.ccm
	monad.ccm
	next_word.ccm
	open_file.ccm
	process.ccm
	read_file.ccm
	skipws.ccm
	spawn.ccm
	tabulate.ccm
	usage_error.ccm
	write_file.ccm
)

if(NIHIL_TESTS)
	add_subdirectory(tests)
	enable_testing()
endif()
