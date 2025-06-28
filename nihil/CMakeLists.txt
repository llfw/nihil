# This source code is released into the public domain.

add_library(nihil STATIC)
target_sources(nihil
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.ccm
	argv.ccm
	command_map.ccm
	ctype.ccm
	ensure_dir.ccm
	errc.ccm
	error.ccm
	exec.ccm
	fd.ccm
	find_in_path.ccm
	format_filesystem.ccm
	generator.ccm
	getenv.ccm
	guard.ccm
	match.ccm
	monad.ccm
	next_word.ccm
	open_file.ccm
	parse_size.ccm
	process.ccm
	read_file.ccm
	rename_file.ccm
	skipws.ccm
	spawn.ccm
	tabulate.ccm
	usage_error.ccm
	write_file.ccm

	PRIVATE
	argv.cc
	command_map.cc
	ensure_dir.cc
	errc.cc
	error.cc
	exec.cc
	fd.cc
	find_in_path.cc
	getenv.cc
	open_file.cc
	process.cc
	rename_file.cc
)

if(NIHIL_TESTS)
	add_subdirectory(tests)
	enable_testing()
endif()
