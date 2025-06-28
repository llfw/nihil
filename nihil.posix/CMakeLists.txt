# This source code is released into the public domain.

add_library(nihil.posix STATIC)
target_link_libraries(nihil.posix PRIVATE nihil.error nihil.guard nihil.monad)

target_sources(nihil.posix
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	nihil.posix.ccm
	argv.ccm
	ensure_dir.ccm
	exec.ccm
	fd.ccm
	open.ccm
	process.ccm
	read_file.ccm
	rename.ccm
	spawn.ccm
	write_file.ccm

	PRIVATE
	argv.cc
	ensure_dir.cc
	exec.cc
	getenv.cc
	fd.cc
	find_in_path.cc
	open.cc
	process.cc
	rename.cc
)

if(NIHIL_TESTS)
	enable_testing()

	add_executable(nihil.posix.test
		test_fd.cc
		test_getenv.cc
		test_spawn.cc
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
