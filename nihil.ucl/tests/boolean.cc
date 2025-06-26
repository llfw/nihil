/*
 * This source code is released into the public domain.
 */

#include <concepts>
#include <string>

#include <catch2/catch_test_macros.hpp>
#include <ucl.h>

import nihil.ucl;

TEST_CASE("ucl: boolean: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	static_assert(std::same_as<bool, boolean::contained_type>);
	REQUIRE(boolean::ucl_type == object_type::boolean);
	REQUIRE(static_cast<::ucl_type>(boolean::ucl_type) == UCL_BOOLEAN);

	static_assert(std::destructible<boolean>);
	static_assert(std::default_initializable<boolean>);
	static_assert(std::move_constructible<boolean>);
	static_assert(std::copy_constructible<boolean>);
	static_assert(std::equality_comparable<boolean>);
	static_assert(std::totally_ordered<boolean>);
	static_assert(std::swappable<boolean>);
}

TEST_CASE("ucl: boolean: default construct", "[ucl]")
{
	auto b = nihil::ucl::boolean();
	REQUIRE(b == false);
}

TEST_CASE("ucl: boolean: construct from value", "[ucl]")
{
	auto b = nihil::ucl::boolean(true);
	REQUIRE(b == true);
}

TEST_CASE("ucl: boolean: swap", "[ucl]")
{
	// do not add using namespace nihil::ucl
	
	auto b1 = nihil::ucl::boolean(true);
	auto b2 = nihil::ucl::boolean(false);

	swap(b1, b2);

	REQUIRE(b1 == false);
	REQUIRE(b2 == true);
}

TEST_CASE("ucl: boolean: value()", "[ucl]")
{
	auto b = nihil::ucl::boolean(true);
	REQUIRE(b.value() == true);
}

TEST_CASE("ucl: boolean: key()", "[ucl]")
{
	using namespace nihil::ucl;

	auto err = parse("a_bool = true");
	REQUIRE(err);

	auto obj = *err;
	REQUIRE(object_cast<boolean>(obj["a_bool"])->key() == "a_bool");

	auto b = nihil::ucl::boolean(true);
	REQUIRE(b.key() == "");
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

	auto err = nihil::ucl::parse("value = true"sv);
	REQUIRE(err);

	auto obj = *err;

	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE(*object_cast<nihil::ucl::boolean>(v) == true);
}

TEST_CASE("ucl: boolean: emit", "[ucl]")
{
	auto b = nihil::ucl::boolean(true);
	auto str = std::format("{}", b);
	REQUIRE(str == "true");
}

TEST_CASE("ucl: boolean: parse and emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("bool = true;");
	REQUIRE(ucl);

	auto output = std::string();
	emit(*ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "bool = true;\n");
}
