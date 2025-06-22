/*
 * This source code is released into the public domain.
 */

#include <algorithm>
#include <ranges>
#include <string>

#include <catch2/catch_test_macros.hpp>

import nihil.ucl;

TEST_CASE("ucl: array: construct", "[ucl]")
{
	auto arr = nihil::ucl::array<nihil::ucl::integer>();
	REQUIRE(arr.size() == 0);
}

TEST_CASE("ucl: array: push_back", "[ucl]")
{
	auto arr = nihil::ucl::array<nihil::ucl::integer>();
	REQUIRE(arr.size() == 0);

	arr.push_back(nihil::ucl::integer(1));
	arr.push_back(nihil::ucl::integer(42));
	arr.push_back(nihil::ucl::integer(666));

	REQUIRE(arr.size() == 3);
	REQUIRE(arr[0].value() == 1);
	REQUIRE(arr[1].value() == 42);
	REQUIRE(arr[2].value() == 666);

	REQUIRE_THROWS_AS(arr[3], std::out_of_range);

	REQUIRE(arr.front() == 1);
	REQUIRE(arr.back() == 666);

}

TEST_CASE("ucl: array: compare", "[ucl]")
{
	auto arr = nihil::ucl::array<nihil::ucl::integer>();
	arr.push_back(1);
	arr.push_back(42);
	arr.push_back(666);

	auto arr2 = nihil::ucl::array<nihil::ucl::integer>();
	REQUIRE(arr != arr2);

	arr2.push_back(1);
	arr2.push_back(42);
	arr2.push_back(666);
	REQUIRE(arr == arr2);

	auto arr3 = nihil::ucl::array<nihil::ucl::integer>();
	arr3.push_back(1);
	arr3.push_back(1);
	arr3.push_back(1);
	REQUIRE(arr != arr3);
}

TEST_CASE("ucl: array: iterator", "[ucl]")
{
	auto arr = nihil::ucl::array<nihil::ucl::integer>{1, 42, 666};

	auto it = arr.begin();
	REQUIRE(*it == 1);

	++it;
	REQUIRE(*it == 42);

	++it;
	REQUIRE(*it == 666);

	--it;
	REQUIRE(*it == 42);
}

TEST_CASE("ucl: array: parse", "[ucl]")
{
	using namespace std::literals;

	auto input = "value = [1, 42, 666]"sv;
	auto obj = nihil::ucl::parse(input);
	auto v = obj.lookup("value");

	REQUIRE(v);
	REQUIRE(v->key() == "value");

	auto arr = object_cast<nihil::ucl::array<nihil::ucl::integer>>(*v);
	REQUIRE(arr.size() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 42);
	REQUIRE(arr[2] == 666);
}

TEST_CASE("ucl: array: emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("array = [1, 42, 666];");
	auto output = std::format("{:c}", ucl);
	REQUIRE(output == 
"array [\n"
"    1,\n"
"    42,\n"
"    666,\n"
"]\n");
}

TEST_CASE("ucl: array is a sized_range", "[ucl]")
{
	auto arr = nihil::ucl::array<nihil::ucl::integer>{1, 42, 666};
	static_assert(std::ranges::sized_range<decltype(arr)>);

	auto size = std::ranges::size(arr);
	REQUIRE(size == 3);

	auto begin = std::ranges::begin(arr);
	static_assert(std::random_access_iterator<decltype(begin)>);
	auto end = std::ranges::end(arr);
	static_assert(std::sentinel_for<decltype(end), decltype(begin)>);

	REQUIRE(std::distance(begin, end) == 3);

	auto vec = std::vector<nihil::ucl::integer>();
	std::ranges::copy(arr, std::back_inserter(vec));
	REQUIRE(std::ranges::equal(arr, vec));

	auto arr_as_ints =
		arr | std::views::transform(&nihil::ucl::integer::value);
	auto int_vec = std::vector<nihil::ucl::integer::value_type>();
	std::ranges::copy(arr_as_ints, std::back_inserter(int_vec));
	REQUIRE(int_vec == std::vector<std::int64_t>{1, 42, 666});

}
