/*
 * This source code is released into the public domain.
 */

#include <catch2/catch_test_macros.hpp>

#include <ucl.h>

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
