// This source code is released into the public domain.

#include <unistd.h>

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.posix;
import nihil.util;

namespace {
TEST_CASE("getenv: existing value", "[getenv]")
{
	auto constexpr *name = "NIHIL_TEST_VAR";
	auto constexpr *value = "test is a test";

	REQUIRE(::setenv(name, value, 1) == 0);

	auto const s = nihil::getenv(name);
	REQUIRE(s);
	REQUIRE(*s == value);
}

TEST_CASE("getenv: non-existing value", "[getenv]")
{
	auto constexpr *name = "NIHIL_TEST_VAR";

	REQUIRE(::unsetenv(name) == 0);

	auto const s = nihil::getenv(name);
	REQUIRE(!s);
	REQUIRE(s.error() == std::errc::no_such_file_or_directory);
}

// Force the call to getenv_r() to reallocate.
TEST_CASE("getenv: long value")
{
	auto constexpr *name = "NIHIL_TEST_VAR";
	auto const value = std::string(4096, 'a');

	REQUIRE(::setenv(name, value.c_str(), 1) == 0);

	auto const s = nihil::getenv(name);
	REQUIRE(s);
	REQUIRE(*s == value);
}

} // anonymous namespace
