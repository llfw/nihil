/*
 * This source code is released into the public domain.
 */

#include <concepts>
#include <list>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <ucl.h>

import nihil.ucl;

TEST_CASE("ucl: string: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	static_assert(std::same_as<std::string_view, string::contained_type>);
	REQUIRE(string::ucl_type == object_type::string);
	REQUIRE(static_cast<::ucl_type>(string::ucl_type) == UCL_STRING);

	static_assert(std::destructible<string>);
	static_assert(std::default_initializable<string>);
	static_assert(std::move_constructible<string>);
	static_assert(std::copy_constructible<string>);
	static_assert(std::equality_comparable<string>);
	static_assert(std::totally_ordered<string>);
	static_assert(std::swappable<string>);

	static_assert(std::ranges::contiguous_range<string>);
	static_assert(std::same_as<char, std::ranges::range_value_t<string>>);
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

TEST_CASE("ucl: string: swap", "[ucl]")
{
	// do not add using namespace nihil::ucl
	
	auto s1 = nihil::ucl::string("one");
	auto s2 = nihil::ucl::string("two");

	swap(s1, s2);

	REQUIRE(s1 == "two");
	REQUIRE(s2 == "one");
}

TEST_CASE("ucl: string: value()", "[ucl]")
{
	auto s = nihil::ucl::string("te\"st");
	REQUIRE(s.value() == "te\"st");
}

TEST_CASE("ucl: string: key()", "[ucl]")
{
	using namespace nihil::ucl;

	auto err = parse("a_string = \"test\"");
	REQUIRE(err);

	auto obj = *err;
	REQUIRE(object_cast<string>(obj["a_string"])->key() == "a_string");

	auto s = nihil::ucl::string("test");
	REQUIRE(s.key() == "");
}

TEST_CASE("ucl: string: size", "[ucl]")
{
	using namespace nihil::ucl;

	REQUIRE(string().size() == 0);
	REQUIRE(string("test").size() == 4);
}

TEST_CASE("ucl: string: empty", "[ucl]")
{
	using namespace nihil::ucl;

	REQUIRE(string().empty() == true);
	REQUIRE(string("test").empty() == false);
}

TEST_CASE("ucl: string: iterator", "[ucl]")
{
	auto str = nihil::ucl::string("test");

	auto begin = std::ranges::begin(str);
	static_assert(std::contiguous_iterator<decltype(begin)>);

	auto end = std::ranges::end(str);
	static_assert(std::sentinel_for<decltype(end), decltype(begin)>);

	REQUIRE(*begin == 't');
	++begin;
	REQUIRE(*begin == 'e');
	++begin;
	REQUIRE(*begin == 's');
	++begin;
	REQUIRE(*begin == 't');
	++begin;

	REQUIRE(begin == end);
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

	auto err = nihil::ucl::parse("value = \"te\\\"st\""sv);
	REQUIRE(err);

	auto obj = *err;
	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE(object_cast<nihil::ucl::string>(v) == "te\"st");
}

TEST_CASE("ucl: string: emit", "[ucl]")
{
	auto s = nihil::ucl::string("te\"st");
	auto str = std::format("{}", s);
	REQUIRE(str == "\"te\\\"st\"");
}

TEST_CASE("ucl: string: parse and emit", "[ucl]")
{
	auto ucl = nihil::ucl::parse("str = \"te\\\"st\";");
	REQUIRE(ucl);

	auto output = std::string();
	emit(*ucl, nihil::ucl::emitter::configuration,
	     std::back_inserter(output));

	REQUIRE(output == "str = \"te\\\"st\";\n");
}
