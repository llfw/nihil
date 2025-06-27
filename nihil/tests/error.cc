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
	using namespace nihil;

	auto e = error("an error");
	REQUIRE(e.str() == to_string(e));
	REQUIRE(to_string(e) == "an error");
	REQUIRE(std::format("{}", e) == to_string(e));
}

TEST_CASE("error: construct from std::error_condition", "[nihil]")
{
	using namespace nihil;

	auto code = std::make_error_condition(std::errc::invalid_argument);
	auto e = error(code);

	REQUIRE(!e.cause());
	REQUIRE(e.code().has_value() == false);
	REQUIRE(e.condition().has_value() == true);

	REQUIRE(e == std::errc::invalid_argument);
	REQUIRE(e != std::errc::no_such_file_or_directory);

	REQUIRE(e.str() == to_string(e));
	REQUIRE(to_string(e) == std::strerror(EINVAL));
	REQUIRE(std::format("{}", e) == to_string(e));
}

TEST_CASE("error: construct from std::errc", "[nihil]")
{
	using namespace nihil;

	auto e = error(std::errc::invalid_argument);

	REQUIRE(!e.cause());
	REQUIRE(e.code().has_value() == false);
	REQUIRE(e.condition().has_value() == true);

	REQUIRE(e == std::errc::invalid_argument);
	REQUIRE(e != std::errc::no_such_file_or_directory);

	REQUIRE(e.str() == to_string(e));
	REQUIRE(to_string(e) == std::strerror(EINVAL));
	REQUIRE(std::format("{}", e) == to_string(e));
}

TEST_CASE("error: compound error", "[nihil]")
{
	using namespace std::literals;
	using namespace nihil;

	auto e = error("cannot open file",
			error(std::errc::no_such_file_or_directory));

	REQUIRE(e.cause());
	REQUIRE(e.code().has_value() == false);
	REQUIRE(e.condition().has_value() == false);

	REQUIRE(*e.cause() == std::errc::no_such_file_or_directory);
	REQUIRE(e.str() == "cannot open file");
	REQUIRE(to_string(e) == ("cannot open file: "s +
				 std::strerror(ENOENT)));
	REQUIRE(std::format("{}", e) == to_string(e));
}

TEST_CASE("error: operator== with strings", "[nihil]")
{
	using namespace nihil;

	auto e1 = error("error");
	auto e2 = error("error");
	auto e3 = error("an error");

	REQUIRE(e1 == e2);
	REQUIRE(e1 != e3);
}

TEST_CASE("error: operator< with strings", "[nihil]")
{
	using namespace nihil;

	auto e1 = error("aaa");
	auto e2 = error("zzz");

	REQUIRE(e1 < e2);
}

TEST_CASE("error: operator== with a cause", "[nihil]")
{
	using namespace nihil;

	auto e1 = error("error", error("cause 1"));
	auto e2 = error("error", error("cause 2"));

	REQUIRE(e1 == e2);
}

TEST_CASE("error: operator== with error_conditions", "[nihil]")
{
	using namespace nihil;

	auto e1 = error(std::errc::invalid_argument);
	auto e2 = error(std::errc::invalid_argument);
	auto e3 = error(std::errc::permission_denied);

	REQUIRE(e1 == e2);
	REQUIRE(e1 != e3);
}

TEST_CASE("error: std::format with string", "[nihil]")
{
	using namespace nihil;

	auto err = error("an error");
	REQUIRE(std::format("{}", err) == "an error");
}

TEST_CASE("error: std::format with std::errc", "[nihil]")
{
	using namespace nihil;

	auto err = error(std::errc::invalid_argument);
	REQUIRE(std::format("{}", err) == std::strerror(EINVAL));
}

TEST_CASE("error: std::format with cause", "[nihil]")
{
	using namespace nihil;

	auto err = error("an error", std::errc::invalid_argument);
	REQUIRE(std::format("{}", err) == "an error: Invalid argument");
}

TEST_CASE("error: throw and catch", "[nihil]")
{
	using namespace std::literals;
	using namespace nihil;

	try {
		throw error("oh no", error(std::errc::invalid_argument));
	} catch (std::exception const &exc) {
		REQUIRE(exc.what() == "oh no: Invalid argument"s);
	}
}
