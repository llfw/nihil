/*
 * This source code is released into the public domain.
 */

#include <list>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <ucl.h>

import nihil.ucl;

TEST_CASE("ucl: string: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	static_assert(std::same_as<std::string_view, string::value_type>);
	REQUIRE(string::ucl_type == object_type::string);
	REQUIRE(static_cast<::ucl_type>(string::ucl_type) == UCL_STRING);
}

TEST_CASE("ucl: string: default construct", "[ucl]")
{
	auto str = nihil::ucl::string();
	REQUIRE(str == "");
}

TEST_CASE("ucl: string: construct from string literal", "[ucl]")
{
	auto str = nihil::ucl::string("testing");
	REQUIRE(str == "testing");
}

TEST_CASE("ucl: string: construct from std::string", "[ucl]")
{
	auto str = nihil::ucl::string(std::string("testing"));
	REQUIRE(str == "testing");
}

TEST_CASE("ucl: string: construct from std::string_view", "[ucl]")
{
	auto str = nihil::ucl::string(std::string_view("testing"));
	REQUIRE(str == "testing");
}

TEST_CASE("ucl: string: construct from contiguous range", "[ucl]")
{
	auto s = std::vector{'t', 'e', 's', 't', 'i', 'n', 'g'};
	auto str = nihil::ucl::string(std::from_range, s);
	REQUIRE(str == "testing");
}

TEST_CASE("ucl: string: construct from non-contiguous range", "[ucl]")
{
	auto s = std::list{'t', 'e', 's', 't', 'i', 'n', 'g'};
	auto str = nihil::ucl::string(std::from_range, s);
	REQUIRE(str == "testing");
}

TEST_CASE("ucl: string: construct from contiguous iterator", "[ucl]")
{
	auto s = std::vector{'t', 'e', 's', 't', 'i', 'n', 'g'};
	auto str = nihil::ucl::string(std::ranges::begin(s),
				      std::ranges::end(s));
	REQUIRE(str == "testing");
}

TEST_CASE("ucl: string: construct from non-contiguous iterator", "[ucl]")
{
	auto s = std::list{'t', 'e', 's', 't', 'i', 'n', 'g'};
	auto str = nihil::ucl::string(std::ranges::begin(s),
				      std::ranges::end(s));
	REQUIRE(str == "testing");
}

TEST_CASE("ucl: string: operator==", "[ucl]")
{
	auto str = nihil::ucl::string("testing");

	REQUIRE(str == nihil::ucl::string("testing"));
	REQUIRE(str == std::string_view("testing"));
	REQUIRE(str == std::string("testing"));
	REQUIRE(str == "testing");

	REQUIRE(str != nihil::ucl::string("test"));
	REQUIRE(str != std::string_view("test"));
	REQUIRE(str != std::string("test"));
	REQUIRE(str != "test");
}

TEST_CASE("ucl: string: operator<=>", "[ucl]")
{
	auto str = nihil::ucl::string("testing");

	REQUIRE(str < nihil::ucl::string("zzz"));
	REQUIRE(str < std::string_view("zzz"));
	REQUIRE(str < std::string("zzz"));
	REQUIRE(str < "zzz");

	REQUIRE(str > nihil::ucl::string("aaa"));
	REQUIRE(str > std::string_view("aaa"));
	REQUIRE(str > std::string("aaa"));
	REQUIRE(str > "aaa");
}

TEST_CASE("ucl: string: parse", "[ucl]")
{
	using namespace std::literals;

	auto obj = nihil::ucl::parse("value = \"str\""sv);
	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE(object_cast<nihil::ucl::string>(v) == "str");
}

TEST_CASE("ucl: string: emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("str = \"te\\\"st\";");

	auto output = std::string();
	emit(ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "str = \"te\\\"st\";\n");
}
