// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.core;

namespace {
struct test_tag
{
};
using testflags = nihil::flagset<unsigned, test_tag>;

constexpr auto zero = testflags::bit<0>();
constexpr auto one = testflags::bit<1>();
constexpr auto two = testflags::bit<2>();
constexpr auto twelve = testflags::bit<12>();

TEST_CASE("flagset: invariant", "[nihil]")
{
	static_assert(std::regular<testflags>);
	static_assert(sizeof(testflags) == sizeof(testflags::underlying_type));
}

TEST_CASE("flagset: bit<>", "[nihil]")
{
	REQUIRE(zero.value() == 0x1);
	REQUIRE(one.value() == 0x2);
}

TEST_CASE("flagset: mask<>", "[nihil]")
{
	auto zero_ = testflags::mask<0x1>();
	auto one_ = testflags::mask<0x2>();

	REQUIRE(zero_ == zero);
	REQUIRE(one_ == one);
}

TEST_CASE("flagset: constructor", "[nihil]")
{
	SECTION ("default construct") {
		auto flags = testflags();
		REQUIRE(flags.value() == 0);
	}

	SECTION ("construct from int") {
		auto flags = testflags::from_int(one.value() | zero.value());
		REQUIRE(flags == (one | zero));
	}

	SECTION ("copy construct") {
		auto flags = one;
		auto flags2(flags);
		REQUIRE(flags == flags2);
	}
}

TEST_CASE("flagset: operators", "[nihil]")
{
	SECTION ("operator|") {
		REQUIRE((zero | one).value() == 0x3);
	}

	SECTION ("operator|=") {
		auto flags = zero;
		flags |= one;
		REQUIRE(flags.value() == 0x3);
	}

	SECTION ("operator&") {
		auto flags = zero | one;

		REQUIRE((flags & zero) == zero);
	}

	SECTION ("operator&=") {
		auto flags = zero | one | two;
		REQUIRE(flags.value() == 0x7);
		flags &= (zero | one);
		REQUIRE(flags.value() == 0x3);
	}

	SECTION ("operator^") {
		auto flags = zero | one;
		REQUIRE((flags ^ (one | two)) == (zero | two));
	}

	SECTION ("operator^=") {
		auto flags = zero | one;
		flags ^= (one | two);
		REQUIRE(flags == (zero | two));
	}

	SECTION ("operator~") {
		auto flags = ~zero;
		REQUIRE(flags.value() == ~static_cast<unsigned>(1));
	}

	SECTION ("operator==") {
		auto flags = zero;
		REQUIRE(flags == zero);
		REQUIRE(flags != one);
	}
}

TEST_CASE("flagset: assignment", "[nihil]")
{
	auto flags = zero;
	REQUIRE(flags == zero);

	flags = one;
	REQUIRE(flags == one);
	REQUIRE(flags != zero);
}

TEST_CASE("flagset: format", "[nihil]")
{
	REQUIRE(std::format("{}", testflags()) == "<>");
	REQUIRE(std::format("{}", zero) == "<0>");
	REQUIRE(std::format("{}", one) == "<1>");
	REQUIRE(std::format("{}", zero | one) == "<1,0>");

	REQUIRE(std::format("{}", twelve) == "<12>");
	REQUIRE(std::format("{}", twelve | one) == "<12,1>");
}

TEST_CASE("flagset: ostream operator<<", "[nihil]")
{
	auto write = [](testflags flags) -> std::string {
		auto strm = std::ostringstream();
		strm << flags;
		return strm.str();
	};

	REQUIRE(write(testflags()) == "<>");
	REQUIRE(write(zero) == "<0>");
	REQUIRE(write(one) == "<1>");
	REQUIRE(write(zero | one) == "<1,0>");

	REQUIRE(write(twelve) == "<12>");
	REQUIRE(write(twelve | one) == "<12,1>");
}
} // anonymous namespace
