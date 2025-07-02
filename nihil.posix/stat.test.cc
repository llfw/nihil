// This source code is released into the public domain.

import nihil.std;
import nihil.core;
import nihil.posix;

#include <sys/stat.h>

#include <catch2/catch_test_macros.hpp>

namespace {

SCENARIO("nihil::stat() on an existing file")
{
	GIVEN("A call to stat() on /etc/passwd")
	{
		auto sb = nihil::stat("/etc/passwd");

		THEN("The returned struct is correct")
		{
			REQUIRE(sb);
			REQUIRE(S_ISREG(sb->st_mode));
		}
	}
}

SCENARIO("nihil::stat() on a non-existent file")
{
	GIVEN("A call to stat() on /nonesuchfile")
	{
		auto sb = nihil::stat("/nonesuchfile");

		THEN("std::errc::no_such_file_or_directory is returned")
		{
			REQUIRE(!sb);
			REQUIRE(sb.error() == std::errc::no_such_file_or_directory);
		}
	}
}

SCENARIO("nihil::stat() on an open file descriptor")
{
	GIVEN("An fd referring to /etc/password")
	{
		auto fd = nihil::open("/etc/passwd", nihil::open_read).value();

		WHEN("nihil::stat() is called on the fd")
		{
			auto sb = nihil::stat(fd);

			THEN("The returned struct is correct")
			{
				REQUIRE(sb);
				REQUIRE(S_ISREG(sb->st_mode));
			}
		}
	}
}

} // anonymous namespace
