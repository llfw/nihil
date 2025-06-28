/*
 * This source code is released into the public domain.
 */

#include <coroutine>
#include <ranges>
#include <vector>

#include <catch2/catch_test_macros.hpp>

import nihil.generator;

TEST_CASE("generator: basic", "[generator]") 
{
	auto fn = [] () -> nihil::generator<int> {
		co_yield 1;
		co_yield 2;
		co_yield 3;
	};

	auto values = std::vector<int>();
	std::ranges::copy(fn(), std::back_inserter(values));

	REQUIRE(values == std::vector{1, 2, 3});
}

TEST_CASE("generator: exceptions", "[generator]") 
{
	auto fn = [] () -> nihil::generator<int> {
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
		co_yield nihil::ranges::elements_of(fn1());
	};

	auto values = std::vector<int>();
	std::ranges::copy(fn2(), std::back_inserter(values));

	REQUIRE(values == std::vector{1, 2, 3});
}
