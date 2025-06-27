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

TEST_CASE("ucl: boolean: constructor", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("default") {
		auto b = boolean();
		REQUIRE(b == false);
	}

	SECTION("with value") {
		auto b = boolean(true);
		REQUIRE(b == true);
	}
}

TEST_CASE("ucl: boolean: construct from UCL object", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("ref, correct type") {
		auto uobj = ::ucl_object_frombool(true);

		auto i = boolean(ref, uobj);
		REQUIRE(i == true);

		::ucl_object_unref(uobj);
	}

	SECTION("noref, correct type") {
		auto uobj = ::ucl_object_frombool(true);

		auto i = boolean(noref, uobj);
		REQUIRE(i == true);
	}

	SECTION("ref, wrong type") {
		auto uobj = ::ucl_object_fromint(1);

		REQUIRE_THROWS_AS(boolean(ref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}

	SECTION("noref, wrong type") {
		auto uobj = ::ucl_object_fromint(1);

		REQUIRE_THROWS_AS(boolean(noref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}
}

TEST_CASE("ucl: boolean: make_boolean", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("default value") {
		auto b = make_boolean().value();
		REQUIRE(b == false);
	}

	SECTION("explicit value") {
		auto b = make_boolean(true).value();
		REQUIRE(b == true);
	}
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

TEST_CASE("ucl: boolean: comparison", "[ucl]")
{
	using namespace nihil::ucl;

	auto b = boolean(true);

	SECTION("operator==") {
		REQUIRE(b == true);
		REQUIRE(b == boolean(true));
	}

	SECTION("operator!=") {
		REQUIRE(b != false);
		REQUIRE(b != boolean(false));
	}

	SECTION("operator<") {
		REQUIRE(b <= true);
		REQUIRE(b <= nihil::ucl::boolean(true));
	}

	SECTION("operator>") {
		REQUIRE(b > false);
		REQUIRE(b > nihil::ucl::boolean(false));
	}
}

TEST_CASE("ucl: boolean: parse", "[ucl]")
{
	using namespace nihil::ucl;

	auto obj = parse("value = true").value();

	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE(object_cast<boolean>(v).value() == true);
}

TEST_CASE("ucl: boolean: parse and emit", "[ucl]")
{
	using namespace nihil::ucl;

	auto ucl = parse("bool = true;").value();

	auto output = std::string();
	emit(ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "bool = true;\n");
}

TEST_CASE("ucl: boolean: format", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("bare boolean") {
		auto str = std::format("{}", boolean(true));
		REQUIRE(str == "true");
	}

	SECTION("parsed boolean") {
		auto obj = parse("bool = true;").value();
		auto b = object_cast<boolean>(obj["bool"]).value();

		auto str = std::format("{}", b);
		REQUIRE(str == "true");
	}

	SECTION("with format string") {
		auto str = std::format("{: >5}", boolean(true));
		REQUIRE(str == " true");
	}
}

TEST_CASE("ucl: boolean: print to ostream", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("bare boolean") {
		auto strm = std::ostringstream();
		strm << boolean(true);

		REQUIRE(strm.str() == "true");
	}

	SECTION("parsed boolean") {
		auto obj = parse("bool = true;").value();
		auto i = object_cast<boolean>(obj["bool"]).value();

		auto strm = std::ostringstream();
		strm << i;

		REQUIRE(strm.str() == "true");
	}
}
