// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

#include <ucl.h>

import nihil.std;
import nihil.ucl;

TEST_CASE("ucl object: get_ucl_object", "[ucl]")
{
	auto obj = nihil::ucl::integer(42);

	REQUIRE(obj.get_ucl_object() != nullptr);
	static_assert(std::same_as<::ucl_object_t *,
		      		   decltype(obj.get_ucl_object())>);

	auto const cobj = obj;
	static_assert(std::same_as<::ucl_object_t const *,
		      		   decltype(cobj.get_ucl_object())>);
}

TEST_CASE("ucl object: compare", "[ucl]")
{
	using namespace std::literals;

	auto obj_41 = nihil::ucl::parse("int = 41;"sv);
	REQUIRE(obj_41);

	auto obj_42 = nihil::ucl::parse("int = 42;"sv);
	REQUIRE(obj_42);

	auto obj_42_2 = nihil::ucl::parse("int = 42;"sv);
	REQUIRE(obj_42_2);

	auto obj_43 = nihil::ucl::parse("int = 43;"sv);
	REQUIRE(obj_43);

	REQUIRE(*obj_42 == *obj_42_2);
	REQUIRE(*obj_42 != *obj_43);
	REQUIRE(*obj_42 < *obj_43);
	REQUIRE(*obj_42 > *obj_41);
}
