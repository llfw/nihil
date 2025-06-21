/*
 * This source code is released into the public domain.
 */

#include <catch2/catch_test_macros.hpp>

import nihil;

using namespace std::literals;

TEST_CASE("generic_error: basic", "[generic_error]") {
	try {
		throw nihil::generic_error("{} + {} = {}", 1, 2, 3);
	} catch (nihil::generic_error const &exc) {
		REQUIRE(exc.what() == "1 + 2 = 3"s);
	}
}
