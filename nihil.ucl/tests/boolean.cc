/*
 * This source code is released into the public domain.
 */

#include <string>

#include <catch2/catch_test_macros.hpp>

import nihil.ucl;

TEST_CASE("ucl: boolean: construct", "[ucl]")
{
	auto b = nihil::ucl::boolean(true);
	REQUIRE(b == true);
}

TEST_CASE("ucl: boolean: default construct", "[ucl]")
{
	auto b = nihil::ucl::boolean();
	REQUIRE(b == false);
}

TEST_CASE("ucl: boolean: operator==", "[ucl]")
{
	auto b = nihil::ucl::boolean(true);

	REQUIRE(b == true);
	REQUIRE(b == nihil::ucl::boolean(true));

	REQUIRE(b != false);
	REQUIRE(b != nihil::ucl::boolean(false));
}

TEST_CASE("ucl: boolean: operator<=>", "[ucl]")
{
	auto b = nihil::ucl::boolean(false);

	REQUIRE(b < true);
	REQUIRE(b < nihil::ucl::boolean(true));

	REQUIRE(b >= false);
	REQUIRE(b >= nihil::ucl::boolean(false));
}

TEST_CASE("ucl: boolean: parse", "[ucl]")
{
	using namespace std::literals;

	auto obj = nihil::ucl::parse("value = true"sv);
	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE(object_cast<nihil::ucl::boolean>(v) == true);
}

TEST_CASE("ucl: boolean: emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("bool = true;");

	auto output = std::string();
	emit(ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "bool = true;\n");
}
