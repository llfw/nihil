/*
 * This source code is released into the public domain.
 */

#include <catch2/catch_test_macros.hpp>

import nihil;

TEST_CASE("generic_error: basic", "[generic_error]") {
	using namespace std::literals;

	// C string
	try {
		throw nihil::generic_error("test error");
	} catch (nihil::generic_error const &exc) {
		REQUIRE(exc.what() == "test error"sv);
	}

	// std::string
	try {
		throw nihil::generic_error("test error"s);
	} catch (nihil::generic_error const &exc) {
		REQUIRE(exc.what() == "test error"sv);
	}

	// std::string_view
	try {
		throw nihil::generic_error("test error"sv);
	} catch (nihil::generic_error const &exc) {
		REQUIRE(exc.what() == "test error"sv);
	}
}
