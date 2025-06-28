# This source code is released into the public domain.

add_library(nihil.posix STATIC)
target_link_libraries(nihil.posix PRIVATE
	nihil.error nihil.flagset nihil.guard nihil.monad)

target_sources(nihil.posix
	PUBLIC FILE_SET modules TYPE CXX_MODULES FILES
	posix.ccm

	posix.argv.ccm
	posix.ensure_dir.ccm
	posix.exec.ccm
	posix.fd.ccm
	posix.getenv.ccm
	posix.open.ccm
	posix.process.ccm
	posix.read_file.ccm
	posix.rename.ccm
	posix.spawn.ccm
	posix.tempfile.ccm
	posix.write_file.ccm

	PRIVATE
	posix.argv.cc
	posix.ensure_dir.cc
	posix.exec.cc
	posix.getenv.cc
	posix.fd.cc
	posix.find_in_path.cc
	posix.open.cc
	posix.process.cc
	posix.rename.cc
	posix.tempfile.cc
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
