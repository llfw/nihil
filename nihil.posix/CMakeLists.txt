# This source code is released into the public domain.

add_library(nihil.posix STATIC)
target_link_libraries(nihil.posix PRIVATE
	nihil.std
	nihil.core
)

target_sources(nihil.posix
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	posix.ccm

	argv.ccm
	ensure_dir.ccm
	execl.ccm
	execlp.ccm
	execshell.ccm
	execv.ccm
	execvp.ccm
	executor.ccm
	fd.ccm
	find_in_path.ccm
	getenv.ccm
	open.ccm
	open_in_path.ccm
	paths.ccm
	process.ccm
	progname.ccm
	read_file.ccm
	rename.ccm
	spawn.ccm
	stat.ccm
	tempfile.ccm
	unistd.ccm
	unlink.ccm
	write_file.ccm
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.posix.test
		argv.test.cc
		execl.test.cc
		execlp.test.cc
		execshell.test.cc
		execv.test.cc
		execvp.test.cc
		fd.test.cc
		find_in_path.test.cc
		getenv.test.cc
		open.test.cc
		open_in_path.test.cc
		stat.test.cc
		tempfile.test.cc
	)

	target_link_libraries(nihil.posix.test PRIVATE
		nihil.std
		nihil.core
		nihil.posix
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.posix.test)
endif()
