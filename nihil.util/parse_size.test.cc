// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.core;
import nihil.error;
import nihil.util;

TEST_CASE("parse_size: empty value", "[nihil]")
{
	using namespace nihil;

	auto n = parse_size<std::uint64_t>("");
	REQUIRE(!n);
	REQUIRE(n.error() == nihil::errc::empty_string);
}

TEST_CASE("parse_size: basic", "[nihil]")
{
	using namespace nihil;

	SECTION("bare number") {
		auto n = parse_size<std::uint64_t>("1024").value();
		REQUIRE(n == 1024);
	}

	SECTION("max value, unsigned") {
		auto n = parse_size<std::uint16_t>("65535").value();
		REQUIRE(n == 65535);
	}

	SECTION("max value, signed") {
		auto n = parse_size<std::uint16_t>("32767").value();
		REQUIRE(n == 32767);
	}

	SECTION("overflow by 1, unsigned") {
		auto n = parse_size<std::uint16_t>("65536");
		REQUIRE(!n);
		REQUIRE(n.error() == std::errc::result_out_of_range);
	}

	SECTION("overflow by 1, signed") {
		auto n = parse_size<std::int16_t>("32768");
		REQUIRE(!n);
		REQUIRE(n.error() == std::errc::result_out_of_range);
	}

	SECTION("overflow by many, unsigned") {
		auto n = parse_size<std::uint16_t>("100000");
		REQUIRE(!n);
		REQUIRE(n.error() == std::errc::result_out_of_range);
	}

	SECTION("overflow by many, signed") {
		auto n = parse_size<std::int16_t>("100000");
		REQUIRE(!n);
		REQUIRE(n.error() == std::errc::result_out_of_range);
	}
}

TEST_CASE("parse_size: invalid multiplier", "[nihil]")
{
	using namespace nihil;

	auto n = parse_size<std::uint64_t>("4z");
	REQUIRE(!n);
	REQUIRE(n.error() == nihil::errc::invalid_unit);

	n = parse_size<std::uint64_t>("4kz");
	REQUIRE(!n);
	REQUIRE(n.error() == nihil::errc::invalid_unit);
}

TEST_CASE("parse_size: multipliers", "[nihil]")
{
	using namespace nihil;

	auto sf = static_cast<std::uint64_t>(4);

	SECTION("k") {
		auto n = parse_size<std::uint64_t>("4k").value();
		REQUIRE(n == sf * 1024);
	}

	SECTION("m") {
		auto n = parse_size<std::uint64_t>("4m").value();
		REQUIRE(n == sf * 1024 * 1024);
	}

	SECTION("g") {
		auto n = parse_size<std::uint64_t>("4g").value();
		REQUIRE(n == sf * 1024 * 1024 * 1024);
	}

	SECTION("t") {
		auto n = parse_size<std::uint64_t>("4t").value();
		REQUIRE(n == sf * 1024 * 1024 * 1024 * 1024);
	}

	SECTION("p") {
		auto n = parse_size<std::uint64_t>("4p").value();
		REQUIRE(n == sf * 1024 * 1024 * 1024 * 1024 * 1024);
	}
}

TEST_CASE("parse_size: multiplier overflow", "[nihil]")
{
	using namespace nihil;

	SECTION("signed") {
		auto n = parse_size<std::uint16_t>("64k");
		REQUIRE(!n);
		REQUIRE(n.error() == std::errc::result_out_of_range);
	}

	SECTION("unsigned") {
		auto n = parse_size<std::int16_t>("32k");
		REQUIRE(!n);
		REQUIRE(n.error() == std::errc::result_out_of_range);
	}
}

TEST_CASE("parse_size: wide", "[nihil]")
{
	using namespace nihil;

	SECTION("bare number") {
		auto n = parse_size<std::uint64_t>(L"1024").value();
		REQUIRE(n == 1024);
	}
}

TEST_CASE("parse_size: wide multipliers", "[nihil]")
{
	using namespace nihil;

	auto sf = static_cast<std::uint64_t>(4);

	SECTION("k") {
		auto n = parse_size<std::uint64_t>(L"4k").value();
		REQUIRE(n == sf * 1024);
	}

	SECTION("m") {
		auto n = parse_size<std::uint64_t>(L"4m").value();
		REQUIRE(n == sf * 1024 * 1024);
	}

	SECTION("g") {
		auto n = parse_size<std::uint64_t>(L"4g").value();
		REQUIRE(n == sf * 1024 * 1024 * 1024);
	}

	SECTION("t") {
		auto n = parse_size<std::uint64_t>(L"4t").value();
		REQUIRE(n == sf * 1024 * 1024 * 1024 * 1024);
	}

	SECTION("p") {
		auto n = parse_size<std::uint64_t>(L"4p").value();
		REQUIRE(n == sf * 1024 * 1024 * 1024 * 1024 * 1024);
	}
}
