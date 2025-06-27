/*
 * This source code is released into the public domain.
 */

#include <vector>

#include <catch2/catch_test_macros.hpp>

import nihil;

namespace {

auto cmd_sub1_called = false;
auto cmd_sub1 = nihil::command("cmd sub1", "", [](int, char **) -> int
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

	int ret = nihil::dispatch_command(args.size() - 1, argv);
	REQUIRE(ret == 0);
	REQUIRE(cmd_sub1_called == true);
}
