/*
 * This source code is released into the public domain.
 */

#include <string>

#include <catch2/catch_test_macros.hpp>

import nihil.ucl;

TEST_CASE("ucl: string: construct", "[ucl]")
{
	auto obj = nihil::ucl::string("testing");
	REQUIRE(object_cast<nihil::ucl::string>(obj).value() == "testing");
}

TEST_CASE("ucl: string: parse", "[ucl]")
{
	using namespace std::literals;

	auto input = "value = \"str\""sv;
	auto obj = nihil::ucl::parse(input);
	auto v = obj.lookup("value");
	REQUIRE(v);
	REQUIRE(v->key() == "value");
	REQUIRE(object_cast<nihil::ucl::string>(*v).value() == "str");
}

TEST_CASE("ucl: string: emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("str = \"te\\\"st\";");

	auto output = std::string();
	emit(ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "str = \"te\\\"st\";\n");
}
