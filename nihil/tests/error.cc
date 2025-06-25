/*
 * This source code is released into the public domain.
 */

#include <cerrno>
#include <cstring>
#include <system_error>

#include <catch2/catch_test_macros.hpp>

import nihil;

TEST_CASE("error: invariants", "[nihil]")
{
	static_assert(std::destructible<nihil::error>);
	static_assert(std::default_initializable<nihil::error>);
	static_assert(std::move_constructible<nihil::error>);
	static_assert(std::copy_constructible<nihil::error>);
	static_assert(std::equality_comparable<nihil::error>);
	static_assert(std::totally_ordered<nihil::error>);
	static_assert(std::swappable<nihil::error>);
	static_assert(std::regular<nihil::error>);
}

TEST_CASE("error: construct from string", "[nihil]")
{
	auto e = nihil::error("an error");
	REQUIRE(e.str() == e.what());
	REQUIRE(e.what() == "an error");
	REQUIRE(std::format("{}", e) == e.what());
}

TEST_CASE("error: construct from std::error_condition", "[nihil]")
{
	auto code = std::make_error_condition(std::errc::invalid_argument);
	auto e = nihil::error(code);

	REQUIRE(e.cause().has_value() == false);
	REQUIRE(e.code().has_value() == false);
	REQUIRE(e.condition().has_value() == true);

	REQUIRE(e == std::errc::invalid_argument);
	REQUIRE(e != std::errc::no_such_file_or_directory);

	REQUIRE(e.str() == e.what());
	REQUIRE(e.what() == std::strerror(EINVAL));
	REQUIRE(std::format("{}", e) == e.what());
}

TEST_CASE("error: construct from std::errc", "[nihil]")
{
	auto e = nihil::error(std::errc::invalid_argument);

	REQUIRE(e.cause().has_value() == false);
	REQUIRE(e.code().has_value() == false);
	REQUIRE(e.condition().has_value() == true);

	REQUIRE(e == std::errc::invalid_argument);
	REQUIRE(e != std::errc::no_such_file_or_directory);

	REQUIRE(e.str() == e.what());
	REQUIRE(e.what() == std::strerror(EINVAL));
	REQUIRE(std::format("{}", e) == e.what());
}

TEST_CASE("error: compound error", "[nihil]")
{
	using namespace std::literals;

	auto e = nihil::error("cannot open file",
			nihil::error(std::errc::no_such_file_or_directory));

	REQUIRE(e.cause().has_value() == true);
	REQUIRE(e.code().has_value() == false);
	REQUIRE(e.condition().has_value() == false);

	REQUIRE(e.cause() == std::errc::no_such_file_or_directory);
	REQUIRE(e.str() == "cannot open file");
	REQUIRE(e.what() == ("cannot open file: "s + std::strerror(ENOENT)));
	REQUIRE(std::format("{}", e) == e.what());
}

TEST_CASE("error: operator== with strings", "[nihil]")
{
	auto e1 = nihil::error("error");
	auto e2 = nihil::error("error");
	auto e3 = nihil::error("an error");

	REQUIRE(e1 == e2);
	REQUIRE(e1 != e3);
}

TEST_CASE("error: operator< with strings", "[nihil]")
{
	auto e1 = nihil::error("aaa");
	auto e2 = nihil::error("zzz");

	REQUIRE(e1 < e2);
}

TEST_CASE("error: operator== with a cause", "[nihil]")
{
	auto e1 = nihil::error("error", nihil::error("cause 1"));
	auto e2 = nihil::error("error", nihil::error("cause 2"));

	REQUIRE(e1 == e2);
}

TEST_CASE("error: operator== with error_conditions", "[nihil]")
{
	auto e1 = nihil::error(std::errc::invalid_argument);
	auto e2 = nihil::error(std::errc::invalid_argument);
	auto e3 = nihil::error(std::errc::permission_denied);

	REQUIRE(e1 == e2);
	REQUIRE(e1 != e3);
}

TEST_CASE("error: std::format with string", "[nihil]")
{
	auto err = nihil::error("an error");
	REQUIRE(std::format("{}", err) == "an error");
}

TEST_CASE("error: std::format with std::errc", "[nihil]")
{
	auto err = nihil::error(std::errc::invalid_argument);
	REQUIRE(std::format("{}", err) == std::strerror(EINVAL));
}

TEST_CASE("error: std::format with cause", "[nihil]")
{
	auto err = nihil::error("an error", std::errc::invalid_argument);
	REQUIRE(std::format("{}", err) == "an error: Invalid argument");
}

