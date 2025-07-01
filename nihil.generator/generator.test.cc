// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.generator;

namespace {
inline auto constexpr test_tags = "[nihil][nihil.generator]";

SCENARIO("A generator that yields values", test_tags)
{
	GIVEN ("A generator that yields values") {
		auto fn = [&]() -> nihil::generator<int> {
			co_yield 1;
			co_yield 2;
		};

		THEN ("The generator yields the original values") {
			REQUIRE(std::ranges::equal(fn(), std::vector{1, 2}));
		}
	}
}

SCENARIO("A generator that yields references", test_tags)
{
	GIVEN ("A generator that yields references") {
		auto one = 1, two = 2;
		auto fn = [&]() -> nihil::generator<int &> {
			co_yield one;
			co_yield two;
		};
		auto range = fn();

		THEN ("The references refer to the original values") {
			auto it = std::ranges::begin(range);
			REQUIRE(&*it == &one);
			++it;
			REQUIRE(&*it == &two);
			++it;
			REQUIRE(it == std::ranges::end(range));
		}
	}
}

SCENARIO("A generator that yields pointers", test_tags)
{
	GIVEN ("A generator that yields pointers") {
		auto one = 1, two = 2;
		auto fn = [&]() -> nihil::generator<int *> {
			co_yield &one;
			co_yield &two;
		};

		THEN ("The pointers point to the original values") {
			REQUIRE(std::ranges::equal(fn(), std::vector{&one, &two}));
		}
	}
}

TEST_CASE("generator: exceptions", "[generator]")
{
	auto fn = []() -> nihil::generator<int> {
		co_yield 1;
		throw std::runtime_error("test");
	};

	auto range = fn();
	auto it = std::ranges::begin(range);
	REQUIRE(*it == 1);
	REQUIRE_THROWS_AS(it++, std::runtime_error);
}

TEST_CASE("generator: elements_of", "[generator]")
{
	auto fn1 = [] -> nihil::generator<int> {
		co_yield 1;
		co_yield 2;
		co_yield 3;
	};

	auto fn2 = [&fn1] -> nihil::generator<int> {
		co_yield nihil::elements_of(fn1());
	};

	auto values = std::vector<int>();
	std::ranges::copy(fn2(), std::back_inserter(values));

	REQUIRE(values == std::vector{1, 2, 3});
}
} // anonymous namespace
