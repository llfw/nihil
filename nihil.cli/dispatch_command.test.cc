// This source code is released into the public domain.

#include <sysexits.h>

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.cli;
import nihil.core;

namespace {

auto cmd_sub1_called = false;
auto cmd_sub1 = nihil::command("cmd sub1", "", [](int, char **) -> int
{
	cmd_sub1_called = true;
	return 0;
});

auto cmd_sub2_called = false;
auto cmd_sub2 = nihil::command("cmd sub2", "", [](int, char **) -> int
{
	cmd_sub2_called = true;
	return 0;
});

} // anonymous namespace

TEST_CASE("nihil.cli: dispatch_command: basic", "[nihil.cli]")
{
	SECTION("cmd sub1") {
		auto args = std::vector<char const *>{
			"cmd", "sub1", nullptr
		};
		auto *argv = const_cast<char **>(args.data());

		auto const ret = nihil::dispatch_command(
				static_cast<int>(args.size()) - 1, argv);
		REQUIRE(ret == 0);
		REQUIRE(cmd_sub1_called == true);
		REQUIRE(cmd_sub2_called == false);
	}

	SECTION("cmd sub2") {
		auto args = std::vector<char const *>{
			"cmd", "sub2", nullptr
		};
		auto *argv = const_cast<char **>(args.data());

		auto const ret = nihil::dispatch_command(
				static_cast<int>(args.size()) - 1, argv);
		REQUIRE(ret == 0);
		REQUIRE(cmd_sub2_called == true);
	}
}

TEST_CASE("nihil.cli: dispatch_command: unknown command", "[nihil.cli]")
{
	auto args = std::vector<char const *>{
		"nocomd", "sub", nullptr
	};
	auto *argv = const_cast<char **>(args.data());

	auto output = std::string();
	auto ret = int{};
	{
		auto capture = nihil::capture_stream(std::cerr);
		ret = nihil::dispatch_command(
				static_cast<int>(args.size()) - 1, argv);
		std::cerr.flush();
		output = capture.str();
	}

	REQUIRE(ret == EX_USAGE);

	auto const *progname = ::getprogname();
	REQUIRE(output == std::format("{}: usage:\n  cmd\n", progname));
}

TEST_CASE("nihil.cli: dispatch_command: incomplete command", "[nihil.cli]")
{
	auto args = std::vector<char const *>{
		"cmd", nullptr
	};
	auto *argv = const_cast<char **>(args.data());

	auto output = std::string();
	auto ret = int{};
	{
		auto capture = nihil::capture_stream(std::cerr);
		ret = nihil::dispatch_command(
				static_cast<int>(args.size()) - 1, argv);
		std::cerr.flush();
		output = capture.str();
	}

	REQUIRE(ret == EX_USAGE);

	auto const *progname = ::getprogname();
	REQUIRE(output == std::format("{}: usage:\n  cmd sub1\n  cmd sub2\n",
				      progname));
}
