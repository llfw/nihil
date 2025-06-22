/*
 * This source code is released into the public domain.
 */

#include <string>

#include <catch2/catch_test_macros.hpp>

import nihil.ucl;

TEST_CASE("ucl: integer: construct", "[ucl]")
{
	auto i = nihil::ucl::integer(42);
	REQUIRE(i == 42);
}

TEST_CASE("ucl: integer: default construct", "[ucl]")
{
	auto i = nihil::ucl::integer();
	REQUIRE(i == 0);
}

TEST_CASE("ucl: integer: operator==", "[ucl]")
{
	auto i = nihil::ucl::integer(42);

	REQUIRE(i == 42);
	REQUIRE(i == nihil::ucl::integer(42));

	REQUIRE(i != 1);
	REQUIRE(i != nihil::ucl::integer(1));
}

TEST_CASE("ucl: integer: operator<=>", "[ucl]")
{
	auto i = nihil::ucl::integer(42);

	REQUIRE(i < 43);
	REQUIRE(i < nihil::ucl::integer(43));

	REQUIRE(i > 1);
	REQUIRE(i > nihil::ucl::integer(1));
}

TEST_CASE("ucl: parse: integer", "[ucl]")
{
	using namespace std::literals;

	auto input = "value = 42"sv;
	auto obj = nihil::ucl::parse(input);
	auto v = obj.lookup("value");
	REQUIRE(v);
	REQUIRE(v->key() == "value");
	REQUIRE(object_cast<nihil::ucl::integer>(*v).value() == 42);
}

TEST_CASE("ucl: integer: emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("int = 42;");

	auto output = std::string();
	emit(ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "int = 42;\n");
}
