// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.cli;

namespace {
inline constexpr auto *test_tags = "[nihil][nihil.cli][nihil.cli.command]";

TEST_CASE("nihil::command invariants", test_tags)
{
	static_assert(!std::move_constructible<nihil::command>);
	static_assert(!std::copy_constructible<nihil::command>);
	static_assert(!std::is_copy_assignable_v<nihil::command>);
	static_assert(!std::is_move_assignable_v<nihil::command>);

	static_assert(std::destructible<nihil::command>);
}

SCENARIO("A command has a path", test_tags)
{
	GIVEN ("A command object with a path") {
		auto cmd = nihil::command("foo bar baz");

		THEN ("The path is correct") {
			REQUIRE(cmd.path() == "foo bar baz");
		}
	}
}

SCENARIO("A command has a handler", test_tags)
{
	GIVEN ("A command object with a handler") {
		auto handler_called = false;
		auto cmd = nihil::command("foo bar baz", "foo bar baz -x", [&](int, char **) {
			handler_called = true;
			return 0;
		});

		THEN ("The usage is correct") {
			REQUIRE(cmd.usage() == "foo bar baz -x");
		}

		AND_WHEN ("The command is invoked") {
			auto ret = cmd.invoke(0, nullptr);

			THEN ("The return value is 0") {
				REQUIRE(ret.value() == 0);
			}
			AND_THEN ("The handler was called") {
				REQUIRE(handler_called == true);
			}
		}
	}
}
} // namespace
