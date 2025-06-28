/*
 * This source code is released into the public domain.
 */

#include <coroutine>
#include <expected>
#include <optional>

#include <catch2/catch_test_macros.hpp>

import nihil.error;
import nihil.monad;

TEST_CASE("monad: co_await std::optional<> with value", "[nihil]")
{
	auto get_value = [] -> std::optional<int> {
		return 42;
	};

	auto try_get_value = [&get_value] -> std::optional<int> {
		co_return co_await get_value();
	};

	auto o = try_get_value();
	REQUIRE(o == 42);
}

TEST_CASE("monad: co_await std::optional<> without value", "[nihil]")
{
	auto get_value = [] -> std::optional<int> {
		return {};
	};

	auto try_get_value = [&get_value] -> std::optional<int> {
		co_return co_await get_value();
	};

	auto o = try_get_value();
	REQUIRE(!o.has_value());
}

TEST_CASE("monad: co_await std::expected<> with value", "[nihil]")
{
	auto get_value = [] -> std::expected<int, std::string> {
		return 42;
	};

	auto try_get_value = [&get_value] -> std::expected<int, std::string> {
		co_return co_await get_value();
	};

	auto o = try_get_value();
	REQUIRE(o == 42);
}

TEST_CASE("monad: co_await std::expected<> with error", "[nihil]")
{
	auto get_value = [] -> std::expected<int, std::string> {
		return std::unexpected("error");
	};

	auto try_get_value = [&get_value] -> std::expected<int, std::string> {
		co_return co_await get_value();
	};

	auto o = try_get_value();
	REQUIRE(!o);
	REQUIRE(o.error() == "error");
}
