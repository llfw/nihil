/*
 * This source code is released into the public domain.
 */

#include <format>
#include <sstream>

#include <catch2/catch_test_macros.hpp>

import nihil;
import nihil.ucl;

TEST_CASE("ucl: emit to std::ostream", "[ucl]")
{
	using namespace std::literals;

	auto obj = nihil::ucl::parse("int = [1, 42, 666]"sv);
	auto strm = std::ostringstream();
	strm << obj;

	// The ostream emitter produces JSON.
	REQUIRE(strm.str() == std::format("{:j}", obj));
}

TEST_CASE("ucl: emit JSON with std::format", "[ucl]")
{
	using namespace std::literals;

	auto obj = nihil::ucl::parse("int = [1, 42, 666]"sv);
	auto str = std::format("{:j}", obj);

	REQUIRE(str ==
"{\n"
"    \"int\": [\n"
"        1,\n"
"        42,\n"
"        666\n"
"    ]\n"
"}");

	// Make sure JSON is the default format.
	auto str2 = std::format("{}", obj);
	REQUIRE(str == str2);
}

TEST_CASE("ucl: emit compact JSON with std::format", "[ucl]")
{
	using namespace std::literals;

	auto obj = nihil::ucl::parse("int = [1, 42, 666]"sv);
	auto str = std::format("{:J}", obj);

	REQUIRE(str == "{\"int\":[1,42,666]}");
}

TEST_CASE("ucl: emit configuration with std::format", "[ucl]")
{
	using namespace std::literals;

	auto obj = nihil::ucl::parse("int = [1, 42, 666]"sv);
	auto str = std::format("{:c}", obj);

	REQUIRE(str ==
"int [\n"
"    1,\n"
"    42,\n"
"    666,\n"
"]\n");
}

TEST_CASE("ucl: emit YAML with std::format", "[ucl]")
{
	using namespace std::literals;

	auto obj = nihil::ucl::parse("int = [1, 42, 666]"sv);
	auto str = std::format("{:y}", obj);

	REQUIRE(str ==
"int: [\n"
"    1,\n"
"    42,\n"
"    666\n"
"]");
}
