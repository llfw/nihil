/*
 * This source code is released into the public domain.
 */

#include <string>

#include <catch2/catch_test_macros.hpp>

import nihil.config;

TEST_CASE("nihil.config: string option", "[nihil][nihil.config]")
{
	std::string storage;

	auto opt = nihil::config::get_option("test_option");
	REQUIRE(!opt);

	{
		auto string_option = nihil::config::string(
			storage, "test_option", "This is a test option");

		auto opt = nihil::config::get_option("test_option");
		REQUIRE(opt);

		REQUIRE((*opt)->name() == "test_option");
		REQUIRE((*opt)->description() == "This is a test option");
		REQUIRE((*opt)->is_default() == true);
		REQUIRE((*opt)->string() == "");

		REQUIRE((*opt)->string("testing"));
		REQUIRE(storage == "testing");
	}

	opt = nihil::config::get_option("test_option");
	REQUIRE(!opt);
}
