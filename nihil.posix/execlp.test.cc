// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.posix;

namespace {

SCENARIO("nihil::execlp() can be used to spawn a shell")
{
	GIVEN("An execlp object")
	{
		auto exec = nihil::execlp("sh", "sh", "-c", "x=1; echo $x");

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

SCENARIO("nihil::execlp() returns the shell's exit code")
{
	GIVEN("An execlp object")
	{
		auto exec = nihil::execlp("sh", "sh", "-c", "x=42; exit $x");

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

SCENARIO("nihil::execlp() returns an error if the executable is not in $PATH")
{
	GIVEN("An execlp object for a non-existent executable")
	{
		auto exec = nihil::execlp("nihil_no_such_executable", "x");

		THEN("A no_such_file_or_directory error is returned")
		{
			REQUIRE(!exec);
			REQUIRE(exec.error().root_cause() == std::errc::no_such_file_or_directory);
		}
	}
}

} // anonymous namespace
