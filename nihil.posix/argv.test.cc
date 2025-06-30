/*
 * This source code is released into the public domain
 */

#include <algorithm>
#include <string>
#include <type_traits>
#include <vector>

#include <catch2/catch_test_macros.hpp>

import nihil.posix;

namespace {
constexpr auto *test_tags = "[nihil][nihil.posix][nihil.posix.argv]";

TEST_CASE("nihil.posix: argv: invariants", test_tags)
{
	static_assert(std::movable<nihil::argv>);
	static_assert(std::move_constructible<nihil::argv>);
	static_assert(std::is_nothrow_move_constructible_v<nihil::argv>);
	static_assert(std::is_nothrow_move_assignable_v<nihil::argv>);

	static_assert(!std::copy_constructible<nihil::argv>);

	static_assert(std::destructible<nihil::argv>);

	static_assert(std::swappable<nihil::argv>);
	static_assert(std::is_nothrow_swappable_v<nihil::argv>);

	static_assert(std::constructible_from<nihil::argv,
		std::initializer_list<std::string_view>>);
	static_assert(std::constructible_from<nihil::argv,
		std::initializer_list<std::string>>);
	static_assert(std::constructible_from<nihil::argv,
		std::initializer_list<char const *>>);
	static_assert(std::constructible_from<nihil::argv,
		std::initializer_list<char *>>);

	static_assert(std::ranges::sized_range<nihil::argv>);
	static_assert(std::ranges::contiguous_range<nihil::argv>);
}

SCENARIO("nihil::argv::size() works")
{
	GIVEN("An argv constructed from 3 elements")
	{
		auto argv = nihil::argv{"one", "two", "three"};

		THEN("size() returns 4")
		{
			REQUIRE(std::ranges::size(argv) == 4);
		}
	}
}

SCENARIO("nihil::argv::operator[] works")
{
	using namespace std::literals;

	GIVEN("An argv constructed from 3 elements")
	{
		auto argv = nihil::argv{"one", "two", "three"};

		THEN("operator[] returns the expected elements")
		{
			REQUIRE(argv[0] == "one"sv);
			REQUIRE(argv[1] == "two"sv);
			REQUIRE(argv[2] == "three"sv);
			REQUIRE(argv[3] == nullptr);
		}

		AND_THEN("operator[] on a non-existent element throws std::out_of_range")
		{
			REQUIRE_THROWS_AS(argv[4], std::out_of_range);
		}
	}
}

SCENARIO("nihil::argv can be constructed from an initializer_list<char const *>")
{
	using namespace std::literals;

	GIVEN("An argv constructed from an initializer_list")
	{
		auto argv = nihil::argv{"one", "two", "three"};

		THEN("size() returns 4")
		{
			REQUIRE(std::ranges::size(argv) == 4);
		}

		AND_THEN("The stored values match the initializer_list")
		{
			REQUIRE(argv[0] == "one"sv);
			REQUIRE(argv[1] == "two"sv);
			REQUIRE(argv[2] == "three"sv);
			REQUIRE(argv[3] == nullptr);
		}
	}
}

SCENARIO("nihil::argv can be constructed from an initializer_list<std::string_view>")
{
	using namespace std::literals;

	GIVEN("An argv constructed from an initializer_list")
	{
		auto argv = nihil::argv{"one"sv, "two"sv, "three"sv};

		THEN("size() returns 4")
		{
			REQUIRE(std::ranges::size(argv) == 4);
		}

		AND_THEN("The stored values match the initializer_list")
		{
			REQUIRE(argv[0] == "one"sv);
			REQUIRE(argv[1] == "two"sv);
			REQUIRE(argv[2] == "three"sv);
			REQUIRE(argv[3] == nullptr);
		}
	}
}

SCENARIO("nihil::argv can be constructed from a range of std::string")
{
	using namespace std::literals;

	GIVEN("An argv constructed from an initializer_list")
	{
		auto vec = std::vector{"one"s, "two"s, "three"s};
		auto argv = nihil::argv(std::from_range, vec);

		THEN("size() returns 4")
		{
			REQUIRE(std::ranges::size(argv) == 4);
		}

		AND_THEN("The stored values match the range")
		{
			REQUIRE(argv[0] == "one"sv);
			REQUIRE(argv[1] == "two"sv);
			REQUIRE(argv[2] == "three"sv);
			REQUIRE(argv[3] == nullptr);
		}
	}
}

SCENARIO("nihil::data() returns the correct data")
{
	using namespace std::literals;

	GIVEN("An argv")
	{
		auto argv = nihil::argv{"one", "two", "three"};

		THEN("The values in data() match the provided data")
		{
			REQUIRE(argv.data()[0] == "one"sv);
			REQUIRE(argv.data()[1] == "two"sv);
			REQUIRE(argv.data()[2] == "three"sv);
			REQUIRE(argv.data()[3] == nullptr);
		}
	}
}

SCENARIO("nihil::argv can be used as a range")
{
	using namespace std::literals;

	GIVEN("An argv")
	{
		auto argv = nihil::argv{"one"sv, "two"sv, "three"sv};

		WHEN("An std::vector is constructed from the argv")
		{
			auto vec = std::vector(std::from_range, argv);

			THEN("The argv and the vector contain the same data")
			{
				REQUIRE(std::ranges::equal(argv, vec));
			}
		}

		WHEN("The argv is copied to an std::vector using a range-based for")
		{
			auto vec = std::vector<char *>();

			for (auto &&arg: argv)
				vec.push_back(arg);

			THEN("The argv and the vector contain the same data")
			{
				REQUIRE(std::ranges::equal(argv, vec));
			}
		}
	}
}

SCENARIO("nihil::argv can be move-constructed")
{
	using namespace std::literals;

	GIVEN("An argv object")
	{
		auto argv = nihil::argv{"one", "two", "three"};

		WHEN("The argv is moved")
		{
			auto argv2 = std::move(argv);

			THEN("The new object contains the data")
			{
				REQUIRE(argv2.size() == 4);
				REQUIRE(argv2[0] == "one"sv);
			}

			AND_THEN("The old object does not")
			{
				REQUIRE(argv.size() == 0);
			}
		}
	}
}

SCENARIO("nihil::argv can be move-assigned")
{
	using namespace std::literals;

	GIVEN("Two argv objects")
	{
		auto argv1 = nihil::argv{"one", "two", "three"};
		auto argv2 = nihil::argv{"x", "y"};

		WHEN("argv2 is move-assigned to argv1")
		{
			argv1 = std::move(argv2);

			THEN("argv1 contains the data from argv2")
			{
				REQUIRE(argv1.size() == 3);
				REQUIRE(argv1[0] == "x"sv);
				REQUIRE(argv1[1] == "y"sv);
				REQUIRE(argv1[2] == nullptr);
			}

			AND_THEN("argv2 is empty")
			{
				REQUIRE(argv2.size() == 0);
			}
		}
	}
}

} // anonymous namespace
