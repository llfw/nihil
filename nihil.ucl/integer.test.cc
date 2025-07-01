// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>
#include <ucl.h>

import nihil.std;
import nihil.ucl;

namespace {
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

TEST_CASE("ucl: integer: constructor", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("default") {
		auto i = integer();
		REQUIRE(i == 0);
	}

	SECTION("with value") {
		auto i = integer(42);
		REQUIRE(i == 42);
	}
}

TEST_CASE("ucl: integer: literal", "[ucl]")
{
	SECTION("with namespace nihil::ucl::literals") {
		using namespace nihil::ucl::literals;

		auto i = 42_ucl;
		REQUIRE(i.type() == nihil::ucl::object_type::integer);
		REQUIRE(i == 42);
	}

	SECTION("with namespace nihil::literals") {
		using namespace nihil::literals;

		auto i = 42_ucl;
		REQUIRE(i.type() == nihil::ucl::object_type::integer);
		REQUIRE(i == 42);
	}
}

TEST_CASE("ucl: integer: construct from UCL object", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("ref, correct type") {
		auto *uobj = ::ucl_object_fromint(42);

		auto i = integer(ref, uobj);
		REQUIRE(i == 42);

		::ucl_object_unref(uobj);
	}

	SECTION("noref, correct type") {
		auto *uobj = ::ucl_object_fromint(42);

		auto i = integer(noref, uobj);
		REQUIRE(i == 42);
	}

	SECTION("ref, wrong type") {
		auto *uobj = ::ucl_object_frombool(true);

		REQUIRE_THROWS_AS(integer(ref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}

	SECTION("noref, wrong type") {
		auto *uobj = ::ucl_object_frombool(true);

		REQUIRE_THROWS_AS(integer(noref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}
}

TEST_CASE("ucl: integer: make_integer", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("default value") {
		auto i = make_integer().value();
		REQUIRE(i == 0);
	}

	SECTION("explicit value") {
		auto i = make_integer(42).value();
		REQUIRE(i == 42);
	}
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
	using namespace nihil::ucl;

	auto i = 42_ucl;
	REQUIRE(i.value() == 42);
}

TEST_CASE("ucl: integer: key()", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("parsed with key") {
		auto obj = parse("an_int = 42").value();
		auto i = object_cast<integer>(obj["an_int"]).value();
		REQUIRE(i.key() == "an_int");
	}

	SECTION("bare integer, no key") {
		auto i = 42_ucl;
		REQUIRE(i.key().empty() == true);
	}
}

TEST_CASE("ucl: integer: comparison", "[ucl]")
{
	using namespace nihil::ucl;

	auto i = 42_ucl;

	SECTION("operator==") {
		REQUIRE(i == 42);
		REQUIRE(i == 42_ucl);
	}

	SECTION("operator!=") {
		REQUIRE(i != 1);
		REQUIRE(i != 1_ucl);
	}

	SECTION("operator<") {
		REQUIRE(i < 43);
		REQUIRE(i < 43_ucl);
	}

	SECTION("operator>") {
		REQUIRE(i > 1);
		REQUIRE(i > 1_ucl);
	}
}

TEST_CASE("ucl: integer: parse", "[ucl]")
{
	using namespace nihil::ucl;

	auto obj = parse("value = 42").value();

	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE(object_cast<integer>(v) == 42);
}

TEST_CASE("ucl: integer: parse and emit", "[ucl]")
{
	using namespace nihil::ucl;

	auto ucl = parse("int = 42;").value();

	auto output = std::string();
	emit(ucl, emitter::configuration, std::back_inserter(output));

	REQUIRE(output == "int = 42;\n");
}

TEST_CASE("ucl: integer: format", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("bare integer") {
		auto str = std::format("{}", 42_ucl);
		REQUIRE(str == "42");
	}

	SECTION("parsed integer") {
		auto obj = parse("int = 42;").value();
		auto i = object_cast<integer>(obj["int"]).value();

		auto str = std::format("{}", i);
		REQUIRE(str == "42");
	}

	SECTION("with format string") {
		auto str = std::format("{:-05}", 42_ucl);
		REQUIRE(str == "00042");
	}
}

TEST_CASE("ucl: integer: print to ostream", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("bare integer") {
		auto strm = std::ostringstream();
		strm << 42_ucl;

		REQUIRE(strm.str() == "42");
	}

	SECTION("parsed integer") {
		auto obj = parse("int = 42;").value();
		auto i = object_cast<integer>(obj["int"]).value();

		auto strm = std::ostringstream();
		strm << i;

		REQUIRE(strm.str() == "42");
	}
}
} // anonymous namespace
