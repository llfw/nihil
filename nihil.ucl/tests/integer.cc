/*
 * This source code is released into the public domain.
 */

#include <concepts>
#include <cstdint>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <ucl.h>

import nihil.ucl;

TEST_CASE("ucl: integer: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	static_assert(std::same_as<std::int64_t, integer::contained_type>);
	REQUIRE(integer::ucl_type == object_type::integer);
	REQUIRE(static_cast<::ucl_type>(integer::ucl_type) == UCL_INT);

	static_assert(std::destructible<integer>);
	static_assert(std::default_initializable<integer>);
	static_assert(std::move_constructible<integer>);
	static_assert(std::copy_constructible<integer>);
	static_assert(std::equality_comparable<integer>);
	static_assert(std::totally_ordered<integer>);
	static_assert(std::swappable<integer>);
}

TEST_CASE("ucl: integer: default construct", "[ucl]")
{
	auto i = nihil::ucl::integer();
	REQUIRE(i == 0);
}

TEST_CASE("ucl: integer: construct", "[ucl]")
{
	auto i = nihil::ucl::integer(42);
	REQUIRE(i == 42);
}

TEST_CASE("ucl: integer: swap", "[ucl]")
{
	// do not add using namespace nihil::ucl
	
	auto i1 = nihil::ucl::integer(1);
	auto i2 = nihil::ucl::integer(2);

	swap(i1, i2);

	REQUIRE(i1 == 2);
	REQUIRE(i2 == 1);
}

TEST_CASE("ucl: integer: value()", "[ucl]")
{
	auto i = nihil::ucl::integer(42);
	REQUIRE(i.value() == 42);
}

TEST_CASE("ucl: integer: key()", "[ucl]")
{
	using namespace nihil::ucl;

	auto err = parse("an_int = 42");
	REQUIRE(err);

	auto obj = *err;
	REQUIRE(object_cast<integer>(obj["an_int"])->key() == "an_int");

	auto i = nihil::ucl::integer(42);
	REQUIRE(i.key() == "");
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

TEST_CASE("ucl: integer: parse", "[ucl]")
{
	using namespace std::literals;

	auto err = nihil::ucl::parse("value = 42"sv);
	REQUIRE(err);

	auto obj = *err;

	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE(object_cast<nihil::ucl::integer>(v) == 42);
}

TEST_CASE("ucl: integer: emit", "[ucl]")
{
	auto i = nihil::ucl::integer(42);
	auto str = std::format("{}", i);
	REQUIRE(str == "42");
}

TEST_CASE("ucl: integer: parse and emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("int = 42;");
	REQUIRE(ucl);

	auto output = std::string();
	emit(*ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "int = 42;\n");
}
