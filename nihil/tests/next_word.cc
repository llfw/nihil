/*
 * This source code is released into the public domain.
 */

#include <locale>
#include <string>

#include <catch2/catch_test_macros.hpp>

import nihil;

TEST_CASE("next_word: basic", "[next_word]")
{
	using namespace std::literals;
	auto s = "foo bar baz"sv;

	auto words = nihil::next_word(s);
	REQUIRE(words.first == "foo");
	REQUIRE(words.second == " bar baz");

	auto word = nihil::next_word(&s);
	REQUIRE(word == "foo");
	REQUIRE(s == " bar baz");
}

TEST_CASE("next_word: multiple spaces", "[next_word]")
{
	using namespace std::literals;
	auto s = "foo  bar  baz"sv;

	auto words = nihil::next_word(s);
	REQUIRE(words.first == "foo");
	REQUIRE(words.second == "  bar  baz");

	auto word = nihil::next_word(&s);
	REQUIRE(word == "foo");
	REQUIRE(s == "  bar  baz");
}

TEST_CASE("next_word: leading spaces", "[next_word]")
{
	using namespace std::literals;
	auto s = " \tfoo  bar  baz"sv;

	auto words = nihil::next_word(s);
	REQUIRE(words.first == "foo");
	REQUIRE(words.second == "  bar  baz");

	auto word = nihil::next_word(&s);
	REQUIRE(word == "foo");
	REQUIRE(s == "  bar  baz");
}

TEST_CASE("next_word: locale", "[next_word]")
{
	using namespace std::literals;
	auto s = L"\u2003foo\u2003bar\u2003baz"sv;

	auto words = nihil::next_word(s);
	REQUIRE(words.first == s);

	words = nihil::next_word(s, std::locale("C.UTF-8"));
	REQUIRE(words.first == L"foo");
	REQUIRE(words.second == L"\u2003bar\u2003baz");
}
