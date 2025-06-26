/*
 * This source code is released into the public domain.
 */

#include <concepts>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ucl.h>

import nihil.ucl;

TEST_CASE("ucl: real: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	static_assert(std::same_as<double, real::contained_type>);
	REQUIRE(real::ucl_type == object_type::real);
	REQUIRE(static_cast<::ucl_type>(real::ucl_type) == UCL_FLOAT);

	static_assert(std::destructible<real>);
	static_assert(std::default_initializable<real>);
	static_assert(std::move_constructible<real>);
	static_assert(std::copy_constructible<real>);
	static_assert(std::equality_comparable<real>);
	static_assert(std::totally_ordered<real>);
	static_assert(std::swappable<real>);
}

TEST_CASE("ucl: real: construct", "[ucl]")
{
	auto obj = nihil::ucl::real(42.1);
	REQUIRE_THAT(object_cast<nihil::ucl::real>(obj)->value(),
		     Catch::Matchers::WithinRel(42.1));
}

TEST_CASE("ucl: real: default construct", "[ucl]")
{
	auto i = nihil::ucl::real();
	REQUIRE(i == 0);
}

TEST_CASE("ucl: real: swap", "[ucl]")
{
	// do not add using namespace nihil::ucl
	
	auto r1 = nihil::ucl::real(1);
	auto r2 = nihil::ucl::real(2);

	swap(r1, r2);

	REQUIRE(r1 == 2.);
	REQUIRE(r2 == 1.);
}

TEST_CASE("ucl: real: operator==", "[ucl]")
{
	auto i = nihil::ucl::real(42.5);

	REQUIRE(i == 42.5);
	REQUIRE(i == nihil::ucl::real(42.5));

	REQUIRE(i != 1);
	REQUIRE(i != nihil::ucl::real(1));
}

TEST_CASE("ucl: real: operator<=>", "[ucl]")
{
	auto i = nihil::ucl::real(42.5);

	REQUIRE(i < 43);
	REQUIRE(i < nihil::ucl::real(43));

	REQUIRE(i > 1);
	REQUIRE(i > nihil::ucl::real(1));
}

TEST_CASE("ucl: real: parse", "[ucl]")
{
	using namespace std::literals;

	auto err = nihil::ucl::parse("value = 42.1"sv);
	REQUIRE(err);

	auto obj = *err;

	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE_THAT(object_cast<nihil::ucl::real>(v)->value(),
		     Catch::Matchers::WithinRel(42.1));
}

TEST_CASE("ucl: real: emit", "[ucl]")
{
	auto err = nihil::ucl::parse("real = 42.2");
	REQUIRE(err);

	auto obj = *err;

	auto output = std::string();
	emit(obj, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "real = 42.2;\n");
}
