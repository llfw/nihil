// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.core;
import nihil.util;

namespace {
inline constexpr auto *test_tags = "[nihil][nihil.error]";

TEST_CASE("error: invariants", test_tags)
{
	static_assert(std::destructible<nihil::error>);
	static_assert(std::default_initializable<nihil::error>);
	static_assert(std::move_constructible<nihil::error>);
	static_assert(std::copy_constructible<nihil::error>);
	static_assert(std::equality_comparable<nihil::error>);
	static_assert(std::totally_ordered<nihil::error>);
	static_assert(std::swappable<nihil::error>);
	static_assert(std::regular<nihil::error>);
}

SCENARIO("A nihil::error can be constructed from a C string", test_tags)
{
	GIVEN ("An error object constructed from a string") {
		auto e = nihil::error("an error");

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "an error");
		}
	}
}

SCENARIO("A nihil::error can be constructed from an std::string lvalue", test_tags)
{
	GIVEN ("An error object constructed from an std::string lvalue") {
		auto s = std::string("an error");
		auto e = nihil::error(s);

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "an error");
		}
	}
}

SCENARIO("A nihil::error can be constructed from an std::string rvalue", test_tags)
{
	GIVEN ("An error object constructed from an std::string rvalue") {
		auto e = nihil::error(std::string("an error"));

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "an error");
		}
	}
}

SCENARIO("A nihil::error can be constructed from an std::string_view lvalue", test_tags)
{
	GIVEN ("An error object constructed from an std::string_view lvalue") {
		auto s = std::string_view("an error");
		auto e = nihil::error(s);

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "an error");
		}
	}
}

SCENARIO("A nihil::error can be constructed from an std::string_view rvalue", test_tags)
{
	GIVEN ("An error object constructed from an std::string_view rvalue") {
		auto e = nihil::error(std::string_view("an error"));

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "an error");
		}
	}
}

SCENARIO("A nihil::error can be constructed from an std::error_condition", test_tags)
{
	GIVEN ("An error object constructed from std::errc::invalid_argument") {
		auto e = nihil::error(std::error_condition(std::errc::invalid_argument));

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "Invalid argument");
		}

		AND_THEN ("condition() should return the error code") {
			REQUIRE(e.condition().has_value());
			REQUIRE(*e.condition() == std::errc::invalid_argument);
		}

		AND_THEN ("The error should be comparable to the error code") {
			REQUIRE(e == std::errc::invalid_argument);
		}
	}
}

SCENARIO("A nihil::error can be constructed from an std::errc", test_tags)
{
	GIVEN ("An error object constructed from std::errc::invalid_argument") {
		auto e = nihil::error(std::errc::invalid_argument);

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "Invalid argument");
		}

		AND_THEN ("condition() should return the error code") {
			REQUIRE(e.condition().has_value());
			REQUIRE(*e.condition() == std::errc::invalid_argument);
		}

		AND_THEN ("The error should be comparable to the error code") {
			REQUIRE(e == std::errc::invalid_argument);
		}
	}
}

SCENARIO("A nihil::error can be constructed from a nihil::errc", test_tags)
{
	GIVEN ("An error object constructed from std::errc::invalid_argument") {
		auto e = nihil::error(nihil::errc::incomplete_command);

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "Incomplete command");
		}

		AND_THEN ("condition() should return the error code") {
			REQUIRE(e.condition().has_value());
			REQUIRE(*e.condition() == nihil::errc::incomplete_command);
		}

		AND_THEN ("The error should be comparable to the error code") {
			REQUIRE(e == nihil::errc::incomplete_command);
		}
	}
}

SCENARIO("A nihil::error can be constructed with a cause", test_tags)
{
	GIVEN ("An error object constructed with a cause") {
		auto e = nihil::error("an error", std::errc::invalid_argument);

		THEN ("full_str() should return the string") {
			REQUIRE(e.full_str() == "an error: Invalid argument");
		}

		AND_THEN ("cause() should return the cause") {
			REQUIRE(e.cause());
			REQUIRE(*e.cause() == std::errc::invalid_argument);
		}
	}
}

SCENARIO("std::format with a nihil::error", test_tags)
{
	GIVEN ("A nihil::error with no cause") {
		auto e = nihil::error("an error");

		THEN ("std::format should return the string") {
			REQUIRE(std::format("{}", e) == "an error");
		}

		AND_THEN ("std::format should return the same as full_str()") {
			REQUIRE(std::format("{}", e) == e.full_str());
		}
	}

	GIVEN ("A nihil::error with a cause") {
		auto e = nihil::error("an error", std::errc::invalid_argument);

		THEN ("std::format should return the string") {
			REQUIRE(std::format("{}", e) == "an error: Invalid argument");
		}

		AND_THEN ("std::format should return the same as full_str()") {
			REQUIRE(std::format("{}", e) == e.full_str());
		}
	}
}

SCENARIO("Print a nihil::error to an std::ostream", test_tags)
{
	GIVEN ("A nihil::error with no cause") {
		auto e = nihil::error("an error");

		THEN ("The error should be printed to the stream") {
			auto ss = std::stringstream();
			ss << e;
			REQUIRE(ss.str() == "an error");
		}
	}

	GIVEN ("A nihil::error with a cause") {
		auto e = nihil::error("an error", std::errc::invalid_argument);

		THEN ("The error should be printed to the stream") {
			auto ss = std::stringstream();
			ss << e;
			REQUIRE(ss.str() == "an error: Invalid argument");
		}
	}
}

SCENARIO("Comparison of nihil::error with operator==", test_tags)
{
	GIVEN ("Two nihil::error objects constructed from the same string") {
		auto e1 = nihil::error("an error");
		auto e2 = nihil::error("an error");

		THEN ("The two objects should be equal") {
			REQUIRE(e1 == e2);
		}
	}

	GIVEN ("Two nihil::error objects constructed from different strings") {
		auto e1 = nihil::error("an error");
		auto e2 = nihil::error("another error");

		THEN ("The two objects should not be equal") {
			REQUIRE(e1 != e2);
		}
	}

	GIVEN ("Two nihil::error objects constructed from the same error code") {
		auto e1 = nihil::error(std::errc::invalid_argument);
		auto e2 = nihil::error(std::errc::invalid_argument);

		THEN ("The two objects should be equal") {
			REQUIRE(e1 == e2);
		}
	}

	GIVEN ("Two nihil::error objects constructed from different error codes") {
		auto e1 = nihil::error(std::errc::invalid_argument);
		auto e2 = nihil::error(std::errc::permission_denied);

		THEN ("The two objects should not be equal") {
			REQUIRE(e1 != e2);
		}
	}
}

SCENARIO("Comparison of nihil::error with operator<", test_tags)
{
	GIVEN ("Two nihil::error objects constructed from the same string") {
		auto e1 = nihil::error("aaa");
		auto e2 = nihil::error("zzz");

		THEN ("aaa should be less than zzz") {
			REQUIRE(e1 < e2);
		}
	}
}

SCENARIO("Throwing and catching a nihil::error object", test_tags)
{
	GIVEN ("A nihil::error object") {
		THEN ("We should be able to throw and catch the error") {
			REQUIRE_THROWS_AS(throw nihil::error("an error"), nihil::error);

			try {
				throw nihil::error("an error");
			} catch (nihil::error const &e) {
				REQUIRE(e.full_str() == "an error");
			};
		}
	}
}

} // anonymous namespace
