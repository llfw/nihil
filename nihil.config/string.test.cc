// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.config;

namespace {
TEST_CASE("nihil.config: string option", "[nihil][nihil.config]")
{
	auto storage = std::string();

	REQUIRE(nihil::config::get_option("test_option").has_value() == false);

	{
		auto string_option = nihil::config::string(
			storage, "test_option", "This is a test option");

		auto *opt = nihil::config::get_option("test_option").value();

		REQUIRE(opt->name() == "test_option");
		REQUIRE(opt->description() == "This is a test option");
		REQUIRE(opt->is_default() == true);
		REQUIRE(opt->string() == "");

		REQUIRE(opt->string("testing"));
		REQUIRE(storage == "testing");
	}

	REQUIRE(nihil::config::get_option("test_option").has_value() == false);
}
} // anonymous namespace
