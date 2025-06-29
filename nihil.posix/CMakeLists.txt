# This source code is released into the public domain.

add_library(nihil.posix STATIC)
target_link_libraries(nihil.posix PRIVATE
	nihil.core nihil.error nihil.flagset nihil.guard nihil.monad)

target_sources(nihil.posix
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	posix.ccm

	argv.ccm
	ensure_dir.ccm
	exec.ccm
	execv.ccm
	executor.ccm
	fd.ccm
	find_in_path.ccm
	getenv.ccm
	open.ccm
	process.ccm
	read_file.ccm
	rename.ccm
	spawn.ccm
	tempfile.ccm
	write_file.ccm

	PRIVATE
	argv.cc
	ensure_dir.cc
	exec.cc
	execv.cc
	getenv.cc
	fd.cc
	find_in_path.cc
	open.cc
	process.cc
	rename.cc
	tempfile.cc
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.posix.test
		test.fd.cc
		test.getenv.cc
		test.spawn.cc
		test.tempfile.cc
	)

	target_link_libraries(nihil.posix.test PRIVATE
		nihil.posix
		Catch2::Catch2WithMain
	)

	find_package(Catch2 REQUIRED)

	include(CTest)
	include(Catch)
	catch_discover_tests(nihil.posix.test)
endif()
