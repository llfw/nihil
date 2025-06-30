// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.posix;

namespace {

SCENARIO("nihil::execvp() can be used to spawn a shell")
{
	GIVEN("An execvp object")
	{
		auto exec = nihil::execvp("sh", nihil::argv{"sh", "-c", "x=1; echo $x"});

		THEN("sh was found in $PATH")
		{
			if (!exec)
				FAIL(exec.error());
		}

		WHEN("The shell is executed")
		{
			auto output = std::string();
			auto capture = nihil::make_capture(nihil::stdout_fileno, output).value();
			auto status = nihil::spawn(exec.value(), capture).value().wait().value();

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

SCENARIO("nihil::execvp() returns the shell's exit code")
{
	GIVEN("An execvp object")
	{
		auto exec = nihil::execvp("sh", nihil::argv{"sh", "-c", "x=42; exit $x"});

		THEN("sh was found in $PATH")
		{
			if (!exec)
				FAIL(exec.error());
		}

		WHEN("The shell is executed")
		{
			auto output = std::string();
			auto capture = nihil::make_capture(nihil::stdout_fileno, output).value();
			auto status = nihil::spawn(exec.value(), capture).value().wait().value();

			THEN("The exit code is 1")
			{
				REQUIRE(status.status() == 42);
			}
		}
	}
}

SCENARIO("nihil::execvp() returns an error if the executable is not in $PATH")
{
	GIVEN("An execvp object for a non-existent executable")
	{
		auto exec = nihil::execvp("nihil_no_such_executable", nihil::argv{"x"});

		THEN("A no_such_file_or_directory error is returned")
		{
			REQUIRE(!exec);
			REQUIRE(exec.error().root_cause() == std::errc::no_such_file_or_directory);
		}
	}
}

} // anonymous namespace
