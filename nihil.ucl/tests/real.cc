/*
 * This source code is released into the public domain.
 */

#include <string>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

import nihil.ucl;

TEST_CASE("ucl: real: construct", "[ucl]")
{
	auto obj = nihil::ucl::real(42.1);
	REQUIRE_THAT(object_cast<nihil::ucl::real>(obj).value(),
		     Catch::Matchers::WithinRel(42.1));
}

TEST_CASE("ucl: real: compare", "[ucl]")
{
	auto i = nihil::ucl::real(42);

	REQUIRE(i == nihil::ucl::real(42));
	REQUIRE(i != nihil::ucl::real(1));
	REQUIRE((i == 42) == true);

	REQUIRE(i > nihil::ucl::real(40));
	REQUIRE(i > 40);
}

TEST_CASE("ucl: real: parse", "[ucl]")
{
	using namespace std::literals;

	auto input = "value = 42.1"sv;
	auto obj = nihil::ucl::parse(input);
	auto v = obj.lookup("value");
	REQUIRE(v);
	REQUIRE(v->key() == "value");
	REQUIRE_THAT(object_cast<nihil::ucl::real>(*v).value(),
		     Catch::Matchers::WithinRel(42.1));
}

TEST_CASE("ucl: real: emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("real = 42.2");

	auto output = std::string();
	emit(ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "real = 42.2;\n");
}
