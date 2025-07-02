// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.core;

TEST_CASE("guard: basic", "[guard]") {
	int n = 0;

	{
		auto guard = nihil::guard([&] { n = 1; });
		REQUIRE(n == 0);
	}

	REQUIRE(n == 1);
}
