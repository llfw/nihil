/*
 * This source code is released into the public domain.
 */

#include <locale>
#include <string>
using namespace std::literals;

#include <catch2/catch_test_macros.hpp>

import nihil.util;

TEST_CASE("skipws: basic", "[skipws]")
{
	REQUIRE(nihil::skipws("foo"sv) == "foo");
	REQUIRE(nihil::skipws("   foo"sv) == "foo");
	REQUIRE(nihil::skipws("foo "sv) == "foo ");
	REQUIRE(nihil::skipws("foo bar"sv) == "foo bar");
}

TEST_CASE("skipws: pointer", "[skipws]")
{
	auto s = "foo"sv;
	nihil::skipws(&s);
	REQUIRE(s == "foo");

	s = "   foo"sv;
	nihil::skipws(&s);
	REQUIRE(s == "foo");

	s = "foo "sv;
	nihil::skipws(&s);
	REQUIRE(s == "foo ");

	s = "foo bar"sv;
	nihil::skipws(&s);
	REQUIRE(s == "foo bar");
}

TEST_CASE("skipws: locale", "[skipws]")
{
	// Assume the default locale is C.
	REQUIRE(nihil::skipws(L"\u2003foo"sv) == L"\u2003foo");
	REQUIRE(nihil::skipws(L"\u2003foo"sv, std::locale("C.UTF-8")) == L"foo");
}
