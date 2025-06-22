/*
 * This source code is released into the public domain.
 */

#include <string>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

import nihil;
import nihil.ucl;

TEST_CASE("ucl parse: iterate array", "[ucl]")
{
	using namespace std::literals;

	auto input = "value = [1, 42, 666];"sv;
	auto obj = nihil::ucl::parse(input);

	auto array = obj.lookup("value");
	REQUIRE(array);
	REQUIRE(array->key() == "value");

	auto vec = std::vector<nihil::ucl::object>();
	std::ranges::copy(*array, std::back_inserter(vec));
	REQUIRE(vec.size() == 3);
	REQUIRE(object_cast<nihil::ucl::integer>(vec[0]).value() == 1);
	REQUIRE(object_cast<nihil::ucl::integer>(vec[1]).value() == 42);
	REQUIRE(object_cast<nihil::ucl::integer>(vec[2]).value() == 666);
}

TEST_CASE("ucl parse: iterate hash", "[ucl]")
{
	using namespace std::literals;

	auto input = "int = 42; bool = true; str = \"test\";"sv;
	auto obj = nihil::ucl::parse(input);

	for (auto &&value : obj) {
		if (value.key() == "int")
			REQUIRE(object_cast<nihil::ucl::integer>(value).value()
				== 42);
		else if (value.key() == "bool")
			REQUIRE(object_cast<nihil::ucl::boolean>(value).value()
				== true);
		else if (value.key() == "str")
			REQUIRE(object_cast<nihil::ucl::string>(value).value()
				== "test");
	}
}
