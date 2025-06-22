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
	using namespace nihil::ucl;

	auto arr = array<integer>();
	REQUIRE(arr.size() == 0);
	REQUIRE(str(arr.type()) == "array");
}

TEST_CASE("ucl: array: push_back", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<integer>();
	REQUIRE(arr.size() == 0);

	arr.push_back(integer(1));
	arr.push_back(integer(42));
	arr.push_back(integer(666));

	REQUIRE(arr.size() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 42);
	REQUIRE(arr[2] == 666);

	REQUIRE_THROWS_AS(arr[3], std::out_of_range);

	REQUIRE(arr.front() == 1);
	REQUIRE(arr.back() == 666);
}

TEST_CASE("ucl: array: compare", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<integer>();
	arr.push_back(integer(1));
	arr.push_back(integer(42));
	arr.push_back(integer(666));

	auto arr2 = array<integer>();
	REQUIRE(arr != arr2);

	arr2.push_back(integer(1));
	arr2.push_back(integer(42));
	arr2.push_back(integer(666));
	REQUIRE(arr == arr2);

	auto arr3 = array<integer>();
	arr3.push_back(integer(1));
	arr3.push_back(integer(1));
	arr3.push_back(integer(1));
	REQUIRE(arr != arr3);
}

TEST_CASE("ucl: array: iterator", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<integer>{integer(1), integer(42), integer(666)};

	auto it = arr.begin();
	REQUIRE(*it == 1);
	auto end = arr.end();
	REQUIRE(it != end);
	REQUIRE(it < end);

	++it;
	REQUIRE(*it == 42);

	++it;
	REQUIRE(*it == 666);

	--it;
	REQUIRE(*it == 42);

	++it;
	REQUIRE(it != end);
	++it;
	REQUIRE(it == end);
}

TEST_CASE("ucl: array: parse", "[ucl]")
{
	using namespace std::literals;
	using namespace nihil::ucl;

	auto input = "value = [1, 42, 666]"sv;
	auto obj = parse(input);
	auto v = obj.lookup("value");

	REQUIRE(v);
	REQUIRE(v->key() == "value");

	auto arr = object_cast<array<integer>>(*v);
	REQUIRE(arr.size() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 42);
	REQUIRE(arr[2] == 666);
}

TEST_CASE("ucl: array: emit", "[ucl]")
{
	using namespace nihil::ucl;

	auto ucl = parse("array = [1, 42, 666];");
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
	using namespace nihil::ucl;

	auto arr = array<integer>{integer(1), integer(42), integer(666)};
	static_assert(std::ranges::sized_range<decltype(arr)>);

	auto size = std::ranges::size(arr);
	REQUIRE(size == 3);

	auto begin = std::ranges::begin(arr);
	static_assert(std::random_access_iterator<decltype(begin)>);

	auto end = std::ranges::end(arr);
	static_assert(std::sentinel_for<decltype(end), decltype(begin)>);

	REQUIRE(std::distance(begin, end) == 3);

	auto vec = std::vector<integer>();
	std::ranges::copy(arr, std::back_inserter(vec));
	REQUIRE(std::ranges::equal(arr, vec));

	auto arr_as_ints =
		arr | std::views::transform(&integer::value);
	auto int_vec = std::vector<integer::value_type>();
	std::ranges::copy(arr_as_ints, std::back_inserter(int_vec));
	REQUIRE(int_vec == std::vector<std::int64_t>{1, 42, 666});

}

TEST_CASE("ucl: array: bad object_cast", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<integer>();

	REQUIRE_THROWS_AS(object_cast<integer>(arr), type_mismatch);
}

TEST_CASE("ucl: array: heterogeneous elements", "[ucl]")
{
	using namespace std::literals;
	using namespace nihil::ucl;

	auto obj = parse("array [ 42, true, \"test\" ];");
	auto v = obj.lookup("array");
	REQUIRE(v);
	auto arr = object_cast<array<>>(*v);

	REQUIRE(arr.size() == 3);

	auto int_obj = object_cast<integer>(arr[0]);
	REQUIRE(int_obj == 42);

	auto bool_obj = object_cast<boolean>(arr[1]);
	REQUIRE(bool_obj == true);

	auto string_obj = object_cast<string>(arr[2]);
	REQUIRE(string_obj == "test");
}

TEST_CASE("ucl: array: heterogenous cast", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<>();
	arr.push_back(integer(42));
	arr.push_back(boolean(true));

	// Converting to an array<integer> should fail.
	REQUIRE_THROWS_AS(object_cast<array<integer>>(arr), type_mismatch);

	// Converting to array<object> should succeed.
	auto obj_arr = object_cast<array<object>>(arr);
	REQUIRE(obj_arr[0] == integer(42));
}

TEST_CASE("ucl: array: homogeneous cast", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<>();
	arr.push_back(integer(1));
	arr.push_back(integer(42));

	auto obj = object(ref, arr.get_ucl_object());

	// Converting to array<string> should fail.
	REQUIRE_THROWS_AS(object_cast<array<string>>(obj), type_mismatch);

	// Converting to an array<integer> should succeed.
	auto obj_arr = object_cast<array<integer>>(obj);
	REQUIRE(obj_arr[0] == 1);
	REQUIRE(obj_arr[1] == 42);
}

