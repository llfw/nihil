// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>
#include <ucl.h>

import nihil.std;
import nihil.ucl;

namespace {
TEST_CASE("ucl: string: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	static_assert(std::same_as<std::string_view, string::contained_type>);
	REQUIRE(string::ucl_type == object_type::string);
	REQUIRE(static_cast<::ucl_type>(string::ucl_type) == UCL_STRING);

	static_assert(std::destructible<string>);
	static_assert(std::default_initializable<string>);
	static_assert(std::move_constructible<string>);
	static_assert(std::copy_constructible<string>);
	static_assert(std::equality_comparable<string>);
	static_assert(std::totally_ordered<string>);
	static_assert(std::swappable<string>);

	static_assert(std::ranges::contiguous_range<string>);
	static_assert(std::same_as<char, std::ranges::range_value_t<string>>);
}

TEST_CASE("ucl: string: literal", "[ucl]")
{
	SECTION("with namespace nihil::ucl::literals") {
		using namespace nihil::ucl::literals;

		auto s = "testing"_ucl;
		REQUIRE(s.type() == nihil::ucl::object_type::string);
		REQUIRE(s == "testing");
	}

	SECTION("with namespace nihil::literals") {
		using namespace nihil::literals;

		auto s = "testing"_ucl;
		REQUIRE(s.type() == nihil::ucl::object_type::string);
		REQUIRE(s == "testing");
	}
}

TEST_CASE("ucl: string: construct", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	SECTION("empty string") {
		auto str = string();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "");
	}

	SECTION("with integer-like value") {
		auto str = "42"_ucl;
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "42");
	}

	SECTION("with boolean-like value") {
		auto str = "true"_ucl;
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "true");
	}

	SECTION("from string literal") {
		auto str = string("testing");
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from std::string") {
		auto str = string("testing"s);
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from std::string_view") {
		auto str = string("testing"sv);
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from contiguous range") {
		auto s = std::vector{'t', 'e', 's', 't', 'i', 'n', 'g'};
		auto str = string(s);
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from non-contiguous range") {
		auto s = std::list{'t', 'e', 's', 't', 'i', 'n', 'g'};
		auto str = string(s);
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from contiguous iterator pair") {
		auto s = std::vector{'t', 'e', 's', 't', 'i', 'n', 'g'};
		auto str = string(s.begin(), s.end());
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from non-contiguous iterator pair") {
		auto s = std::list{'t', 'e', 's', 't', 'i', 'n', 'g'};
		auto str = string(s.begin(), s.end());
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}
}

TEST_CASE("ucl: string: construct from UCL object", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION("ref, correct type") {
		auto *uobj = ::ucl_object_fromstring("testing");

		auto const s = string(ref, uobj);
		REQUIRE(s == "testing");

		::ucl_object_unref(uobj);
	}

	SECTION("noref, correct type") {
		auto *uobj = ::ucl_object_fromstring("testing");

		auto const s = string(noref, uobj);
		REQUIRE(s == "testing");
	}

	SECTION("ref, wrong type") {
		auto *uobj = ::ucl_object_frombool(true);

		REQUIRE_THROWS_AS(string(ref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}

	SECTION("noref, wrong type") {
		auto *uobj = ::ucl_object_frombool(true);

		REQUIRE_THROWS_AS(string(noref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}
}

TEST_CASE("ucl: string: make_string", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	SECTION("empty string") {
		auto const str = make_string().value();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "");
	}

	SECTION("from string literal") {
		auto const str = make_string("testing").value();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from std::string") {
		auto const str = make_string("testing"s).value();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from std::string_view") {
		auto const str = make_string("testing"sv).value();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from contiguous range") {
		auto const s = std::vector{'t', 'e', 's', 't', 'i', 'n', 'g'};
		auto const str = make_string(s).value();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from non-contiguous range") {
		auto const s = std::list{'t', 'e', 's', 't', 'i', 'n', 'g'};
		auto const str = make_string(s).value();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from contiguous iterator pair") {
		auto const s = std::vector{'t', 'e', 's', 't', 'i', 'n', 'g'};
		auto const str = make_string(s.begin(), s.end()).value();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}

	SECTION("from non-contiguous iterator pair") {
		auto const s = std::list{'t', 'e', 's', 't', 'i', 'n', 'g'};
		auto const str = make_string(s.begin(), s.end()).value();
		REQUIRE(str.type() == object_type::string);
		REQUIRE(str == "testing");
	}
}

TEST_CASE("ucl: string: swap", "[ucl]")
{
	// do not add using namespace nihil::ucl
	
	auto s1 = nihil::ucl::string("one");
	auto s2 = nihil::ucl::string("two");

	swap(s1, s2);

	REQUIRE(s1 == "two");
	REQUIRE(s2 == "one");
}

TEST_CASE("ucl: string: value()", "[ucl]")
{
	using namespace nihil::ucl;

	auto const s = string(R"(te"st)");
	REQUIRE(s.value() == R"(te"st)");
}

TEST_CASE("ucl: string: key()", "[ucl]")
{
	using namespace nihil::ucl;

	auto obj = parse(R"(a_string = "test")").value();
	REQUIRE(object_cast<string>(obj["a_string"]).value().key() == "a_string");

	auto const s = string("test");
	REQUIRE(s.key().empty() == true);
}

TEST_CASE("ucl: string: size", "[ucl]")
{
	using namespace nihil::ucl;

	REQUIRE(string().size() == 0);
	REQUIRE(string("test").size() == 4);
}

TEST_CASE("ucl: string: empty", "[ucl]")
{
	using namespace nihil::ucl;

	REQUIRE(string().empty() == true);
	REQUIRE(string("test").empty() == false);
}

TEST_CASE("ucl: string: iterate", "[ucl]")
{
	using namespace nihil::ucl;

	auto str = "test"_ucl;

	SECTION("as iterator pair") {
		auto begin = str.begin();
		static_assert(std::contiguous_iterator<decltype(begin)>);

		auto end = str.end();
		static_assert(std::sentinel_for<decltype(end),
						      decltype(begin)>);

		REQUIRE(*begin == 't');
		++begin;
		REQUIRE(*begin == 'e');
		++begin;
		REQUIRE(*begin == 's');
		++begin;
		REQUIRE(*begin == 't');
		++begin;

		REQUIRE(begin == end);
	}

	SECTION("as range") {
		auto s = std::string(std::from_range, str);
		REQUIRE(s == "test");
	}
}

TEST_CASE("ucl: string: comparison", "[ucl]")
{
	using namespace nihil::ucl;

	auto str = "testing"_ucl;

	SECTION("operator==") {
		REQUIRE(str == "testing"_ucl);
		REQUIRE(str == std::string_view("testing"));
		REQUIRE(str == std::string("testing"));
		REQUIRE(str == "testing");
	}

	SECTION("operator!=") {
		REQUIRE(str != "test"_ucl);
		REQUIRE(str != std::string_view("test"));
		REQUIRE(str != std::string("test"));
		REQUIRE(str != "test");
	}

	SECTION("operator<") {
		REQUIRE(str < "zzz"_ucl);
		REQUIRE(str < std::string_view("zzz"));
		REQUIRE(str < std::string("zzz"));
		REQUIRE(str < "zzz");
	}

	SECTION("operator>") {
		REQUIRE(str > "aaa"_ucl);
		REQUIRE(str > std::string_view("aaa"));
		REQUIRE(str > std::string("aaa"));
		REQUIRE(str > "aaa");
	}
}

TEST_CASE("ucl: string: parse", "[ucl]")
{
	using namespace nihil::ucl;

	auto obj = parse(R"(value = "te\"st")").value();

	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE(object_cast<nihil::ucl::string>(v).value() == "te\"st");
}

TEST_CASE("ucl: string: emit", "[ucl]")
{
	using namespace nihil::ucl;

	auto ucl = parse(R"(str = "te\"st";)").value();

	auto output = std::string();
	emit(ucl, emitter::configuration, std::back_inserter(output));

	REQUIRE(output == "str = \"te\\\"st\";\n");
}

TEST_CASE("ucl: string: format", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto constexpr test_string = "te\"st"sv;

	SECTION("bare string") {
		auto str = std::format("{}", string(test_string));
		REQUIRE(str == test_string);
	}

	SECTION("parsed string") {
		auto obj = parse(R"(string = "te\"st";)").value();
		auto s = object_cast<string>(obj["string"]).value();

		auto str = std::format("{}", s);
		REQUIRE(str == test_string);
	}

	SECTION("with format string") {
		auto str = std::format("{: >10}", string(test_string));
		REQUIRE(str == "     te\"st");
	}
}

TEST_CASE("ucl: string: print to ostream", "[ucl]")
{
	using namespace nihil::ucl;
	using namespace std::literals;

	auto constexpr test_string = "te\"st"sv;

	SECTION("bare string") {
		auto strm = std::ostringstream();
		strm << string(test_string);

		REQUIRE(strm.str() == test_string);
	}

	SECTION("parsed string") {
		auto obj = parse(R"(string = "te\"st";)").value();
		auto s = object_cast<string>(obj["string"]).value();

		auto strm = std::ostringstream();
		strm << s;

		REQUIRE(strm.str() == test_string);
	}

	SECTION("with format string") {
		auto str = std::format("{: >10}", string(test_string));
		REQUIRE(str == "     te\"st");
	}
}
} // anonymous namespace
