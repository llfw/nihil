// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.core;

namespace {
TEST_CASE("nihil.util: capture", "[nihil][nihil.util]")
{
	SECTION("std::cout with release()") {
		auto cap = nihil::capture_stream(std::cout);

		std::cout << 1 << '+' << 1 << '=' << (1 + 1) << '\n';
		REQUIRE(cap.str() == "1+1=2\n");
		
		cap.release();
		REQUIRE(cap.str() == "1+1=2\n");
	}

	SECTION("std::cout with dtor") {
		auto cap = nihil::capture_stream(std::cout);
		std::cout << 1 << '+' << 1 << '=' << (1 + 1) << '\n';
		REQUIRE(cap.str() == "1+1=2\n");
	}

	SECTION("std::cerr with release()") {
		auto cap = nihil::capture_stream(std::cerr);

		std::cerr << 1 << '+' << 1 << '=' << (1 + 1) << '\n';
		REQUIRE(cap.str() == "1+1=2\n");
		
		cap.release();
		REQUIRE(cap.str() == "1+1=2\n");
	}

	SECTION("std::cerr with dtor") {
		auto cap = nihil::capture_stream(std::cerr);
		std::cerr << 1 << '+' << 1 << '=' << (1 + 1) << '\n';
		REQUIRE(cap.str() == "1+1=2\n");
	}
}

} // anonymous namespace
