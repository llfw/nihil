/*
 * This source code is released into the public domain.
 */

#include <string>

#include <catch2/catch_test_macros.hpp>

import nihil.config;

TEST_CASE("nihil.config: string option", "[nihil][nihil.config]")
{
	std::string storage;

	REQUIRE_THROWS_AS(nihil::config::get_option("test_option"),
			  nihil::config::unknown_option);

	{
		auto string_option = nihil::config::string(
			storage, "test_option", "This is a test option");

		auto &opt = nihil::config::get_option("test_option");
		REQUIRE(opt.name() == "test_option");
		REQUIRE(opt.description() == "This is a test option");
		REQUIRE(opt.is_default() == true);
		REQUIRE(opt.string() == "");

		opt.string("testing");
		REQUIRE(storage == "testing");
	}

	REQUIRE_THROWS_AS(nihil::config::get_option("test_option"),
			  nihil::config::unknown_option);
}
