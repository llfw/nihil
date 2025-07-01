// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.cli;

namespace {

inline auto constexpr *test_tags = "[nihil][nihil.cli]";

SCENARIO("Inserting and retrieving nodes in a command_tree", test_tags)
{
	GIVEN ("A command tree") {
		auto tree = nihil::command_tree();

		THEN ("We can insert nodes into the tree") {
			tree.insert(std::make_shared<nihil::command>("cmd1 sub1"));
			tree.insert(std::make_shared<nihil::command>("cmd1 sub2"));
			tree.insert(std::make_shared<nihil::command>("cmd2 sub1"));

			AND_THEN ("We can retrieve nodes from the tree") {
				auto c1s1 = tree.find(std::vector{"cmd1", "sub1"});
				REQUIRE(c1s1.first->command().path() == "cmd1 sub1");

				auto c1s2 = tree.find(std::vector{"cmd1", "sub2"});
				REQUIRE(c1s2.first->command().path() == "cmd1 sub2");

				auto c2s1 = tree.find(std::vector{"cmd2", "sub1"});
				REQUIRE(c2s1.first->command().path() == "cmd2 sub1");
			}

			AND_THEN ("Fetching an unknown command returns the root node") {
				auto cmd = tree.find(std::vector{"cmd3", "sub1"});
				REQUIRE(cmd.first->command().path() == "");
			}

			AND_THEN ("Fetching an unknown subcommand returns the cmd1 node") {
				auto cmd = tree.find(std::vector{"cmd1", "sub3"});
				REQUIRE(cmd.first->command().path() == "cmd1");
			}
		}
	}
}
} // anonymous namespace
