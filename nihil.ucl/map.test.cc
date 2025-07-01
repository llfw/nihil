// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>
#include <ucl.h>

import nihil.std;
import nihil.ucl;

//NOLINTBEGIN(bugprone-unchecked-optional-access)

namespace {
TEST_CASE("ucl: map: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	REQUIRE(map<>::ucl_type == object_type::object);
	REQUIRE(static_cast<::ucl_type>(map<>::ucl_type) == UCL_OBJECT);

	static_assert(std::destructible<map<>>);
	static_assert(std::default_initializable<map<>>);
	static_assert(std::move_constructible<map<>>);
	static_assert(std::copy_constructible<map<>>);
	static_assert(std::equality_comparable<map<>>);
	static_assert(std::totally_ordered<map<>>);
	static_assert(std::swappable<map<>>);

	static_assert(std::ranges::range<map<integer>>);
	static_assert(std::same_as<std::pair<std::string_view, integer>,
				   std::ranges::range_value_t<map<integer>>>);
}

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

TEST_CASE("ucl: map: construct from range", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto vec = std::vector<std::pair<std::string_view, integer>>{
			{"1"sv, integer(1)},
			{"42"sv, integer(42)},
		};

	auto map = nihil::ucl::map<integer>(vec);

	REQUIRE(str(map.type()) == "object");
	REQUIRE(map["1"] == 1);
	REQUIRE(map["42"] == 42);
}

TEST_CASE("ucl: map: construct from iterator pair", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto vec = std::vector<std::pair<std::string_view, integer>>{
			{"1"sv, integer(1)},
			{"42"sv, integer(42)},
		};

	auto map = nihil::ucl::map<integer>(std::ranges::begin(vec),
					    std::ranges::end(vec));

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
	REQUIRE(obj.value() == 42);

	obj = map.find("43");
	REQUIRE(!obj.has_value());
}

TEST_CASE("ucl: map: iterate", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto map = nihil::ucl::map<integer>{
			{"1"sv, integer(1)},
			{"42"sv, integer(42)},
		};

	auto i = 0U;

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
	REQUIRE(obj.value() == 42);

	REQUIRE(!map.find("42"));
	REQUIRE(map["1"] == 1);

	obj = map.pop("42");
	REQUIRE(!obj);
}

} // anonymous namespace

//NOLINTEND(bugprone-unchecked-optional-access)
