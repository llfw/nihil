// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.core;

using namespace std::literals;
using namespace nihil;

namespace {
TEST_CASE("tabulate: basic", "[tabulate]")
{
	auto input = std::vector{
		std::vector{"a",   "foo", "b"},
		std::vector{"bar", "c",   "baz"},
	};

	auto result = std::string();
	tabulate("{:1} {:2} {:3}", input, std::back_inserter(result));
	REQUIRE(result ==
"1   2   3\n"
"a   foo b\n"
"bar c   baz\n");
}

TEST_CASE("tabulate: basic wide", "[tabulate]")
{
	auto input = std::vector{
		std::vector{L"a",   L"foo", L"b"},
		std::vector{L"bar", L"c",   L"baz"},
	};

	auto result = std::wstring();
	wtabulate(L"{:1} {:2} {:3}", input, std::back_inserter(result));

	REQUIRE(result ==
L"1   2   3\n"
"a   foo b\n"
"bar c   baz\n");
}

TEST_CASE("tabulate: jagged", "[tabulate]")
{
	auto input = std::vector{
		std::vector{"a",   "foo", "b"},
		std::vector{"bar", "baz"},
	};

	auto result = std::string();
	tabulate("{:1} {:2} {:3}", input, std::back_inserter(result));
	REQUIRE(result ==
"1   2   3\n"
"a   foo b\n"
"bar baz\n");
}

TEST_CASE("tabulate: align", "[tabulate]")
{
	auto input = std::vector{
		std::vector{"a", "longvalue", "s"},
		std::vector{"a", "s",         "longvalue"},
	};

	auto result = std::string();
	tabulate("{:1} {<:2} {>:3}", input, std::back_inserter(result));
	REQUIRE(result ==
"1 2                 3\n"
"a longvalue         s\n"
"a s         longvalue\n");
}
} // anonymous namespace
