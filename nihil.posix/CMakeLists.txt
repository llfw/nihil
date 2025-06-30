# This source code is released into the public domain.

add_library(nihil.posix STATIC)
target_link_libraries(nihil.posix PRIVATE
	nihil.core nihil.error nihil.flagset nihil.guard nihil.monad)

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
	fexecv.ccm
	fexecvp.ccm
	find_in_path.ccm
	getenv.ccm
	open.ccm
	open_in_path.ccm
	process.ccm
	read_file.ccm
	rename.ccm
	spawn.ccm
	tempfile.ccm
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
		fd.test.cc
		getenv.test.cc
		test.spawn.cc
		tempfile.test.cc
	)

	target_link_libraries(nihil.posix.test PRIVATE
		nihil.posix
		Catch2::Catch2WithMain
	)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.posix.test)
endif()
