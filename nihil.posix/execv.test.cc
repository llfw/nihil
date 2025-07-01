// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.posix;

namespace {

SCENARIO("nihil::execv() can be used to spawn a shell")
{
	GIVEN("An execv object")
	{
		auto exec = nihil::execv("/bin/sh", nihil::argv{"sh", "-c", "x=1; echo $x"});

		WHEN("The shell is executed")
		{
			auto output = std::string();
			auto capture = nihil::make_capture(nihil::stdout_fileno, output).value();
			auto status = nihil::spawn(exec, capture).value().wait().value();

			THEN("The exit code is 0")
			{
				REQUIRE(status.status() == 0);
			}
			AND_THEN("The expected output was captured")
			{
				REQUIRE(output == "1\n");
			}
		}
	}
}

SCENARIO("nihil::execv() returns the shell's exit code")
{
	GIVEN("An execshell object")
	{
		auto exec = nihil::execv("/bin/sh", nihil::argv{"sh", "-c", "x=42; exit $x"});

		WHEN("The shell is executed")
		{
			auto output = std::string();
			auto capture = nihil::make_capture(nihil::stdout_fileno, output).value();
			auto status = nihil::spawn(exec, capture).value().wait().value();

			THEN("The exit code is 1")
			{
				REQUIRE(status.status() == 42);
			}
		}
	}
}

} // anonymous namespace
