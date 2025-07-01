// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

import nihil.std;
import nihil.ucl;

namespace {
TEST_CASE("ucl parse: iterate array", "[ucl]")
{
	using namespace std::literals;
	using namespace nihil::ucl;

	auto err = parse("value = [1, 42, 666];"sv);
	REQUIRE(err);

	auto obj = *err;

	auto arr = obj["value"];
	REQUIRE(arr.key() == "value");

	auto ints = object_cast<array<integer>>(arr);
	REQUIRE(ints);

	auto vec = std::vector(std::from_range, *ints);

	REQUIRE(vec.size() == 3);
	REQUIRE(vec[0] == 1);
	REQUIRE(vec[1] == 42);
	REQUIRE(vec[2] == 666);
}

TEST_CASE("ucl parse: iterate hash", "[ucl]")
{
	using namespace std::literals;
	using namespace nihil::ucl;

	auto input = "int = 42; bool = true; str = \"test\";"sv;
	auto obj = parse(input);
	REQUIRE(obj);

	for (auto &&[key, value] : *obj) {
		REQUIRE(key == value.key());

		if (key == "int")
			REQUIRE(object_cast<integer>(value) == 42);
		else if (key == "bool")
			REQUIRE(object_cast<boolean>(value) == true);
		else if (key == "str")
			REQUIRE(object_cast<string>(value) == "test");
	}
}

} // anonymous namespace
