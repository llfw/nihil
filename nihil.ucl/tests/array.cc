/*
 * This source code is released into the public domain.
 */

#include <algorithm>
#include <concepts>
#include <expected>
#include <ranges>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <ucl.h>

import nihil;
import nihil.ucl;

TEST_CASE("ucl: array: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	REQUIRE(array<>::ucl_type == object_type::array);
	REQUIRE(static_cast<::ucl_type>(array<>::ucl_type) == UCL_ARRAY);

	static_assert(std::destructible<array<>>);
	static_assert(std::default_initializable<array<>>);
	static_assert(std::move_constructible<array<>>);
	static_assert(std::copy_constructible<array<>>);
	static_assert(std::equality_comparable<array<>>);
	static_assert(std::totally_ordered<array<>>);
	static_assert(std::swappable<array<>>);
	
	static_assert(std::ranges::sized_range<array<integer>>);
	static_assert(std::same_as<std::ranges::range_value_t<array<integer>>,
				   integer>);
}

TEST_CASE("ucl: array: default construct", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<integer>();
	REQUIRE(arr.size() == 0);
	REQUIRE(str(arr.type()) == "array");
}

TEST_CASE("ucl: array: construct from range", "[ucl]")
{
	using namespace nihil::ucl;

	auto vec = std::vector{integer(1), integer(42)};
	auto arr = array<integer>(std::from_range, vec);

	REQUIRE(arr.size() == 2);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 42);
}

TEST_CASE("ucl: array: construct from iterator pair", "[ucl]")
{
	using namespace nihil::ucl;

	auto vec = std::vector{integer(1), integer(42)};
	auto arr = array<integer>(std::ranges::begin(vec),
				  std::ranges::end(vec));

	REQUIRE(arr.size() == 2);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 42);
}

TEST_CASE("ucl: array: construct from initializer_list", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<integer>{integer(1), integer(42)};

	REQUIRE(arr.size() == 2);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 42);
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

	auto arr = array<integer>{
		integer(1), integer(42), integer(666)
	};

	auto arr2 = array<integer>();
	REQUIRE(arr != arr2);

	arr2.push_back(integer(1));
	arr2.push_back(integer(42));
	arr2.push_back(integer(666));
	REQUIRE(arr == arr2);

	auto arr3 = array<integer>{
		integer(1), integer(1), integer(1)
	};

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

	auto obj_err = parse("value = [1, 42, 666]"sv);
	REQUIRE(obj_err);
	auto obj = *obj_err;

	auto err = object_cast<array<integer>>(obj["value"]);
	REQUIRE(err);

	auto arr = *err;
	REQUIRE(arr.size() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 42);
	REQUIRE(arr[2] == 666);
}

TEST_CASE("ucl: array: emit", "[ucl]")
{
	using namespace nihil::ucl;

	auto ucl = parse("array = [1, 42, 666];");
	REQUIRE(ucl);

	auto output = std::format("{:c}", *ucl);
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
	auto int_vec = std::vector<integer::contained_type>();
	std::ranges::copy(arr_as_ints, std::back_inserter(int_vec));
	REQUIRE(int_vec == std::vector<std::int64_t>{1, 42, 666});

}

TEST_CASE("ucl: array: bad object_cast", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<integer>();

	auto cast_ok = object_cast<integer>(arr);
	REQUIRE(!cast_ok);
}

TEST_CASE("ucl: array: heterogeneous elements", "[ucl]")
{
	using namespace std::literals;
	using namespace nihil::ucl;

	auto obj_err = parse("array [ 42, true, \"test\" ];");
	REQUIRE(obj_err);
	auto obj = *obj_err;

	auto err = object_cast<array<>>(obj["array"]);
	REQUIRE(err);

	auto arr = *err;
	REQUIRE(arr.size() == 3);

	auto int_obj = object_cast<integer>(arr[0]);
	REQUIRE(int_obj);
	REQUIRE(*int_obj == 42);

	auto bool_obj = object_cast<boolean>(arr[1]);
	REQUIRE(bool_obj);
	REQUIRE(*bool_obj == true);

	auto string_obj = object_cast<string>(arr[2]);
	REQUIRE(string_obj);
	REQUIRE(*string_obj == "test");
}

TEST_CASE("ucl: array: heterogenous cast", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<>();
	arr.push_back(integer(42));
	arr.push_back(boolean(true));

	// Converting to an array<integer> should fail.
	auto cast_ok = object_cast<array<integer>>(arr);
	REQUIRE(!cast_ok);

	// Converting to array<object> should succeed.
	auto err = object_cast<array<object>>(arr);
	REQUIRE(err);

	auto obj_arr = *err;
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
	auto cast_ok = object_cast<array<string>>(obj);
	REQUIRE(!cast_ok);

	// Converting to an array<integer> should succeed.
	auto err = object_cast<array<integer>>(obj);
	REQUIRE(err);

	auto obj_arr = *err;
	REQUIRE(obj_arr[0] == 1);
	REQUIRE(obj_arr[1] == 42);
}

