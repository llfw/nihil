// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.posix;
import nihil.util;

namespace {

SCENARIO("nihil::find_in_path can find /bin/sh in the default path")
{
	GIVEN ("A call to find_in_path for /bin/sh") {
		auto ret = nihil::find_in_path("sh");

		THEN ("The return value should be /bin/sh") {
			REQUIRE(ret);
			REQUIRE(ret.value() == "/bin/sh");
		}
	}
}

SCENARIO("nihil::find_in_path works with an explicit path")
{
	GIVEN ("A call to find_in_path with an explicit path") {
		auto ret = nihil::find_in_path("sh", "/bin:/usr/bin");

		THEN ("The return value should be /bin/sh") {
			REQUIRE(ret);
			REQUIRE(ret.value() == "/bin/sh");
		}
	}
}

SCENARIO("nihil::find_in_path works with an absolute path")
{
	GIVEN ("A call to find_in_path with an absolute filename and an empty path") {
		auto ret = nihil::find_in_path("/bin/sh", "");

		THEN ("The return value should be /bin/sh") {
			REQUIRE(ret);
			REQUIRE(ret.value() == "/bin/sh");
		}
	}
}

SCENARIO("nihil::find_in_path returns ENOENT when the executable is not found")
{
	GIVEN ("A call to find_in_path for a non-existent executable") {
		auto ret = nihil::find_in_path("nihil_no_such_executable");

		THEN ("The return value should be std::errc::no_such_file_or_directory") {
			REQUIRE(!ret);
			REQUIRE(ret.error() == std::errc::no_such_file_or_directory);
		}
	}
}

SCENARIO("nihil::find_in_path returns errors from access(2)")
{
	GIVEN ("A call to find_in_path for a non-executable file") {
		auto ret = nihil::find_in_path("passwd", "/etc");

		THEN ("The return value should be std::errc::permission_denied") {
			REQUIRE(!ret);
			REQUIRE(ret.error() == std::errc::permission_denied);
		}
	}
}

} // anonymous namespace
