// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.util;

namespace {
TEST_CASE("match", "[nihil]")
{
	using namespace nihil;
	using namespace std::literals;

	auto v = std::variant<int, std::string>(42);

	auto s = v | match {
		[](int) { return "int"s; },
		[](std::string const &) { return "string"s; }
	};

	REQUIRE(s == "int");

	v = "test"s;

	s = v | match {
		[](int) { return "int"s; },
		[](std::string const &) { return "string"s; }
	};

	REQUIRE(s == "string");
}
} // anonymous namespace
