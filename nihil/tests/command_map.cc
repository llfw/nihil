/*
 * This source code is released into the public domain.
 */

#include <vector>

#include <catch2/catch_test_macros.hpp>

import nihil;

namespace {

auto cmd_sub1_called = false;
auto cmd_sub1 = nihil::command<int>("cmd sub1", [](int, int, char **) -> int
{
	cmd_sub1_called = true;
	return 0;
});

} // anonymous namespace

TEST_CASE("command_map: basic", "[command_map]")
{
	auto args = std::vector<char const *>{
		"cmd", "sub1", nullptr
	};
	auto argv = const_cast<char **>(args.data());
	nihil::dispatch_command(0, args.size() - 1, argv);
	REQUIRE(cmd_sub1_called == true);
}

TEST_CASE("command_map: no arguments", "[command_map]")
{
	auto args = std::vector<char const *>{
		nullptr
	};
	auto argv = const_cast<char **>(args.data());
	REQUIRE_THROWS_AS(nihil::dispatch_command(0, args.size() - 1, argv),
			  nihil::usage_error);
}

TEST_CASE("command_map: unknown command", "[command_map]")
{
	auto args = std::vector<char const *>{
		"cmd", "nonesuch", nullptr
	};
	auto argv = const_cast<char **>(args.data());

	REQUIRE_THROWS_AS(nihil::dispatch_command(0, args.size() - 1, argv),
			  nihil::usage_error);
}
