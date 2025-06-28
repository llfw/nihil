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

TEST_CASE("ucl: array: constructor", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("default") {
		auto arr = array<integer>();
		REQUIRE(arr.size() == 0);
		REQUIRE(str(arr.type()) == "array");
	}

	SECTION("from range") {
		auto vec = std::vector{integer(1), integer(42)};
		auto arr = array<integer>(std::from_range, vec);

		REQUIRE(arr.size() == 2);
		REQUIRE(arr[0] == 1);
		REQUIRE(arr[1] == 42);
	}

	SECTION("from iterator pair") {
		auto vec = std::vector{integer(1), integer(42)};
		auto arr = array<integer>(std::ranges::begin(vec),
					  std::ranges::end(vec));

		REQUIRE(arr.size() == 2);
		REQUIRE(arr[0] == 1);
		REQUIRE(arr[1] == 42);
	}

	SECTION("from initializer_list") {
		auto arr = array<integer>{integer(1), integer(42)};

		REQUIRE(arr.size() == 2);
		REQUIRE(arr[0] == 1);
		REQUIRE(arr[1] == 42);
	}
}

TEST_CASE("ucl: array: construct from UCL object", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("ref, correct type") {
		auto uarr = ::ucl_object_typed_new(UCL_ARRAY);
		auto uint = ::ucl_object_fromint(42);
		::ucl_array_append(uarr, uint);

		auto arr = array<integer>(ref, uarr);
		REQUIRE(arr[0] == 42);

		::ucl_object_unref(uarr);
	}

	SECTION("noref, correct type") {
		auto uarr = ::ucl_object_typed_new(UCL_ARRAY);
		auto uint = ::ucl_object_fromint(42);
		::ucl_array_append(uarr, uint);

		auto arr = array<integer>(noref, uarr);
		REQUIRE(arr[0] == 42);
	}

	SECTION("ref, wrong element type") {
		auto uarr = ::ucl_object_typed_new(UCL_ARRAY);
		auto uint = ::ucl_object_frombool(true);
		::ucl_array_append(uarr, uint);

		auto arr = array<integer>(noref, uarr);
		REQUIRE_THROWS_AS(arr[0], type_mismatch);
	}

	SECTION("ref, wrong type") {
		auto uobj = ::ucl_object_frombool(true);

		REQUIRE_THROWS_AS(array(ref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}

	SECTION("noref, wrong type") {
		auto uobj = ::ucl_object_frombool(true);

		REQUIRE_THROWS_AS(array(noref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}
}

TEST_CASE("ucl: array: swap", "[ucl]")
{
	// do not add using namespace nihil::ucl

	auto arr1 = nihil::ucl::array<nihil::ucl::integer>{
		nihil::ucl::integer(1),
		nihil::ucl::integer(2)
	};

	auto arr2 = nihil::ucl::array<nihil::ucl::integer>{
		nihil::ucl::integer(3),
	};

	swap(arr1, arr2);

	REQUIRE(arr1.size() == 1);
	REQUIRE(arr1[0] == 3);

	REQUIRE(arr2.size() == 2);
	REQUIRE(arr2[0] == 1);
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

	auto obj = parse("value = [1, 42, 666]"sv).value();

	auto arr = object_cast<array<integer>>(obj["value"]).value();

	REQUIRE(arr.size() == 3);
	REQUIRE(arr[0] == 1);
	REQUIRE(arr[1] == 42);
	REQUIRE(arr[2] == 666);
}

TEST_CASE("ucl: array: emit", "[ucl]")
{
	using namespace nihil::ucl;

	auto ucl = parse("array = [1, 42, 666];").value();

	auto output = std::format("{:c}", ucl);
	REQUIRE(output == 
"array [\n"
"    1,\n"
"    42,\n"
"    666,\n"
"]\n");
}

TEST_CASE("ucl: array: format", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("empty array") {
		auto arr = array<integer>();
		REQUIRE(std::format("{}", arr) == "[]");
	}

	SECTION("bare array") {
		auto arr = array<integer>{
			integer(1), integer(42), integer(666)
		};

		auto output = std::format("{}", arr);
		REQUIRE(output == "[1, 42, 666]");
	}

	SECTION("parsed array") {
		auto ucl = parse("array = [1, 42, 666];").value();
		auto arr = object_cast<array<integer>>(ucl["array"]).value();

		auto output = std::format("{}", arr);
		REQUIRE(output == "[1, 42, 666]");
	}
}

TEST_CASE("ucl: array: print to ostream", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("empty array") {
		auto arr = array<integer>();
		auto strm = std::ostringstream();
		strm << arr;

		REQUIRE(strm.str() == "[]");
	}

	SECTION("bare array") {
		auto arr = array<integer>{
			integer(1), integer(42), integer(666)
		};
		auto strm = std::ostringstream();
		strm << arr;

		REQUIRE(strm.str() == "[1, 42, 666]");
	}

	SECTION("parsed array") {
		auto ucl = parse("array = [1, 42, 666];").value();
		auto arr = object_cast<array<integer>>(ucl["array"]).value();
		auto strm = std::ostringstream();
		strm << arr;

		REQUIRE(strm.str() == "[1, 42, 666]");
	}
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

TEST_CASE("array iterator: empty iterator", "[ucl]")
{
	using namespace nihil::ucl;

	auto it = array_iterator<integer>();

	REQUIRE_THROWS_AS(*it, std::logic_error);
	REQUIRE_THROWS_AS(it[0], std::logic_error);
	REQUIRE_THROWS_AS(it++, std::logic_error);
	REQUIRE_THROWS_AS(++it, std::logic_error);

	auto it2 = array_iterator<integer>();
	REQUIRE(it == it2);
	REQUIRE((it < it2) == false);
	REQUIRE((it > it2) == false);
}

TEST_CASE("array iterator: invalid operations", "[ucl]")
{
	using namespace nihil::ucl;

	auto arr = array<integer>{ integer(42) };
	auto it = arr.begin();

	SECTION("decrement before start") {
		REQUIRE_THROWS_AS(--it, std::logic_error);
		REQUIRE_THROWS_AS(it--, std::logic_error);
		REQUIRE_THROWS_AS(it - 1, std::logic_error);
	}

	SECTION("increment past end") {
		++it;
		REQUIRE(it == arr.end());

		REQUIRE_THROWS_AS(++it, std::logic_error);
		REQUIRE_THROWS_AS(it++, std::logic_error);
		REQUIRE_THROWS_AS(it + 1, std::logic_error);
	}

	SECTION("dereference iterator at end") {
		REQUIRE_THROWS_AS(it[1], std::logic_error);

		++it;
		REQUIRE(it == arr.end());

		REQUIRE_THROWS_AS(*it, std::logic_error);
	}

	SECTION("compare with different array") {
		auto arr2 = array<integer>{ integer(42) };
		REQUIRE_THROWS_AS(it == arr2.begin(), std::logic_error);
		REQUIRE_THROWS_AS(it > arr2.begin(), std::logic_error);
		REQUIRE_THROWS_AS(it - arr2.begin(), std::logic_error);
	}

	SECTION("compare with empty iterator") {
		auto it2 = array_iterator<integer>();
		REQUIRE_THROWS_AS(it == it2, std::logic_error);
		REQUIRE_THROWS_AS(it > it2, std::logic_error);
		REQUIRE_THROWS_AS(it - it2, std::logic_error);
	}
}
