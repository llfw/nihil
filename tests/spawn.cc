/*
 * This source code is released into the public domain.
 */

#include <catch2/catch_test_macros.hpp>

import nihil;

TEST_CASE("spawn: system", "[spawn]") {
	using namespace nihil;
	auto output = std::string();
	auto result = spawn(shell("x=1; echo $x"),
			    capture(stdout_fileno, output)).wait();

	REQUIRE(result.okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execv", "[spawn]") {
	using namespace nihil;
	auto output = std::string();
	auto args = argv::from_args({"sh", "-c", "x=1; echo $x"});
	auto result = spawn(execv("/bin/sh", std::move(args)),
			    capture(stdout_fileno, output)).wait();

	REQUIRE(result.okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execvp", "[spawn]") {
	using namespace nihil;
	auto output = std::string();
	auto args = argv::from_args({"sh", "-c", "x=1; echo $x"});
	auto result = spawn(execvp("sh", std::move(args)),
			    capture(stdout_fileno, output)).wait();

	REQUIRE(result.okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execl", "[spawn]") {
	using namespace nihil;
	auto output = std::string();
	auto result = spawn(execl("/bin/sh", "sh", "-c", "x=1; echo $x"),
			    capture(stdout_fileno, output)).wait();

	REQUIRE(result.okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execlp", "[spawn]") {
	using namespace nihil;
	auto output = std::string();
	auto result = spawn(execlp("sh", "sh", "-c", "x=1; echo $x"),
			    capture(stdout_fileno, output)).wait();

	REQUIRE(result.okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execlp failure", "[spawn]") {
	using namespace nihil;
	REQUIRE_THROWS_AS(execlp("lfjail_nonesuch_executable", "x"),
			  executable_not_found);
}
