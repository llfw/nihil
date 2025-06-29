/*
 * This source code is released into the public domain.
 */

#include <catch2/catch_test_macros.hpp>

import nihil.posix;

TEST_CASE("spawn: system", "[spawn]")
{
	using namespace nihil;

	auto exec = shell("x=1; echo $x");
	REQUIRE(exec);

	auto output = std::string();
	auto capture = make_capture(stdout_fileno, output);
	REQUIRE(capture);

	auto proc = spawn(*exec, *capture);
	REQUIRE(proc);

	auto status = std::move(*proc).wait();
	REQUIRE(status);

	REQUIRE(status->okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execv", "[spawn]") {
	using namespace nihil;

	auto args = argv({"sh", "-c", "x=1; echo $x"});
	auto exec = execv("/bin/sh", std::move(args));

	auto output = std::string();
	auto capture = make_capture(stdout_fileno, output);
	REQUIRE(capture);

	auto proc = spawn(exec, *capture);
	REQUIRE(proc);

	auto status = std::move(*proc).wait();
	if (!status)
		FAIL(to_string(status.error()));

	REQUIRE(status->okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execvp", "[spawn]") {
	using namespace nihil;

	auto args = argv({"sh", "-c", "x=1; echo $x"});
	auto exec = execvp("sh", std::move(args));
	REQUIRE(exec);

	auto output = std::string();
	auto capture = make_capture(stdout_fileno, output);
	REQUIRE(capture);

	auto proc = spawn(*exec, *capture);
	REQUIRE(proc);

	auto status = std::move(*proc).wait();
	REQUIRE(status);

	REQUIRE(status->okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execl", "[spawn]") {
	using namespace nihil;

	auto exec = execl("/bin/sh", "sh", "-c", "x=1; echo $x");
	REQUIRE(exec);

	auto output = std::string();
	auto capture = make_capture(stdout_fileno, output);
	REQUIRE(capture);

	auto proc = spawn(*exec, *capture);
	REQUIRE(proc);

	auto status = std::move(*proc).wait();
	REQUIRE(status);

	REQUIRE(status->okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execlp", "[spawn]") {
	using namespace nihil;

	auto exec = execlp("sh", "sh", "-c", "x=1; echo $x");
	REQUIRE(exec);

	auto output = std::string();
	auto capture = make_capture(stdout_fileno, output);
	REQUIRE(capture);

	auto proc = spawn(*exec, *capture);
	REQUIRE(proc);

	auto status = std::move(*proc).wait();
	REQUIRE(status);

	REQUIRE(status->okay());
	REQUIRE(output == "1\n");
}

TEST_CASE("spawn: execlp failure", "[spawn]") {
	using namespace nihil;

	auto exec = execlp("nihil_no_such_executable", "x");
	REQUIRE(!exec);
}
