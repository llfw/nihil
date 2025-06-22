/*
 * This source code is released into the public domain.
 */

#include <string>

#include <catch2/catch_test_macros.hpp>

import nihil.ucl;

TEST_CASE("ucl: boolean: construct", "[ucl]")
{
	auto obj = nihil::ucl::boolean(true);
	REQUIRE(object_cast<nihil::ucl::boolean>(obj).value() == true);
}

TEST_CASE("ucl: boolean: compare", "[ucl]")
{
	auto b = nihil::ucl::boolean(true);

	REQUIRE(b == nihil::ucl::boolean(true));
	REQUIRE(b == true);

	REQUIRE(b != nihil::ucl::boolean(false));
	REQUIRE(b != false);
}

TEST_CASE("ucl: boolean: parse", "[ucl]")
{
	using namespace std::literals;

	auto input = "value = true"sv;
	auto obj = nihil::ucl::parse(input);

	auto v = obj.lookup("value");
	REQUIRE(v);
	REQUIRE(v->key() == "value");
	REQUIRE(object_cast<nihil::ucl::boolean>(*v).value() == true);
}

TEST_CASE("ucl: boolean: emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("bool = true;");

	auto output = std::string();
	emit(ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "bool = true;\n");
}
