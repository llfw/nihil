// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.posix;
import nihil.util;

namespace {

SCENARIO("nihil::open_in_path can find /bin/sh in the default path")
{
	GIVEN ("A call to open_in_path for /bin/sh") {
		auto ret = nihil::open_in_path("sh");

		THEN ("The returned file descriptor should be /bin/sh") {
			REQUIRE(ret);

			auto sb1 = nihil::stat(*ret).value();
			auto sb2 = nihil::stat("/bin/sh").value();
			REQUIRE(sb1.st_ino == sb2.st_ino);
			REQUIRE(sb1.st_dev == sb2.st_dev);
		}
	}
}

SCENARIO("nihil::open_in_path works with an explicit path")
{
	GIVEN ("A call to open_in_path with an explicit path") {
		auto ret = nihil::open_in_path("sh", "/bin:/usr/bin");

		THEN ("The returned file descriptor should be /bin/sh") {
			REQUIRE(ret);

			auto sb1 = nihil::stat(*ret).value();
			auto sb2 = nihil::stat("/bin/sh").value();
			REQUIRE(sb1.st_ino == sb2.st_ino);
			REQUIRE(sb1.st_dev == sb2.st_dev);
		}
	}
}

SCENARIO("nihil::open_in_path works with an absolute path")
{
	GIVEN ("A call to open_in_path with an absolute filename and an empty path") {
		auto ret = nihil::open_in_path("/bin/sh", "");

		THEN ("The returned file descriptor should be /bin/sh") {
			REQUIRE(ret);

			auto sb1 = nihil::stat(*ret).value();
			auto sb2 = nihil::stat("/bin/sh").value();
			REQUIRE(sb1.st_ino == sb2.st_ino);
			REQUIRE(sb1.st_dev == sb2.st_dev);
		}
	}
}

SCENARIO("nihil::open_in_path returns ENOENT when the executable is not found")
{
	GIVEN ("A call to open_in_path for a non-existent executable") {
		auto ret = nihil::open_in_path("nihil_no_such_executable");

		THEN ("The return value should be std::errc::no_such_file_or_directory") {
			REQUIRE(!ret);
			REQUIRE(ret.error() == std::errc::no_such_file_or_directory);
		}
	}
}

SCENARIO("nihil::open_in_path returns errors from open(2)")
{
	GIVEN ("A call to open_in_path for a non-executable file") {
		auto ret = nihil::open_in_path("passwd", "/etc");

		THEN ("The return value should be std::errc::permission_denied") {
			REQUIRE(!ret);
			REQUIRE(ret.error() == std::errc::permission_denied);
		}
	}
}

} // anonymous namespace
