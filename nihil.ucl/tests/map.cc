/*
 * This source code is released into the public domain.
 */

#include <catch2/catch_test_macros.hpp>

import nihil.ucl;

TEST_CASE("ucl: map: default construct", "[ucl]")
{
	auto map = nihil::ucl::map<>();
	REQUIRE(str(map.type()) == "object");
}

TEST_CASE("ucl: map: construct from initializer_list", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto map = nihil::ucl::map<integer>{
		{"1"sv, integer(1)},
		{"42"sv, integer(42)},
	};

	REQUIRE(str(map.type()) == "object");
	REQUIRE(map["1"] == 1);
	REQUIRE(map["42"] == 42);
}

TEST_CASE("ucl: map: insert", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto m = map<integer>();

	m.insert({"test1"sv, integer(42)});
	m.insert({"test2"sv, integer(666)});

	REQUIRE(m["test1"] == 42);
	REQUIRE(m["test2"] == 666);
}

TEST_CASE("ucl: map: find", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto map = nihil::ucl::map<integer>{
		{"1"sv, integer(1)},
		{"42"sv, integer(42)},
	};

	auto obj = map.find("42");
	REQUIRE(obj != std::nullopt);
	REQUIRE(*obj == 42);

	obj = map.find("43");
	REQUIRE(obj == std::nullopt);
}

TEST_CASE("ucl: map: iterate", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto map = nihil::ucl::map<integer>{
		{"1"sv, integer(1)},
		{"42"sv, integer(42)},
	};

	auto i = 0u;

	for (auto [key, value] : map) {
		if (key == "1")
			REQUIRE(value == 1);
		else if (key == "42")
			REQUIRE(value == 42);
		else
			REQUIRE(false);
		++i;
	}

	REQUIRE(i == 2);
}

TEST_CASE("ucl: map: operator[] throws key_not_found", "[ucl]")
{
	auto map = nihil::ucl::map<nihil::ucl::integer>();
	REQUIRE_THROWS_AS(map["nonesuch"], nihil::ucl::key_not_found);
}

TEST_CASE("ucl: map: remove", "[uc]")
{
	using namespace std::literals;
	using namespace nihil::ucl;

	auto map = nihil::ucl::map<integer>{
		{"1"sv, integer(1)},
		{"42"sv, integer(42)},
	};

	REQUIRE(map.find("42") != std::nullopt);
	REQUIRE(map.remove("42") == true);
	REQUIRE(map.find("42") == std::nullopt);
	REQUIRE(map["1"] == 1);

	REQUIRE(map.remove("42") == false);
}

TEST_CASE("ucl: map: pop", "[uc]")
{
	using namespace std::literals;
	using namespace nihil::ucl;

	auto map = nihil::ucl::map<integer>{
		{"1"sv, integer(1)},
		{"42"sv, integer(42)},
	};

	REQUIRE(map.find("42") != std::nullopt);

	auto obj = map.pop("42");
	REQUIRE(obj != std::nullopt);
	REQUIRE(*obj == 42);

	REQUIRE(map.find("42") == std::nullopt);
	REQUIRE(map["1"] == 1);

	obj = map.pop("42");
	REQUIRE(obj == std::nullopt);
}
