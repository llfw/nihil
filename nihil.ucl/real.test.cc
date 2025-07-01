// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <ucl.h>

import nihil.std;
import nihil.ucl;

namespace {
TEST_CASE("ucl: real: invariants", "[ucl]")
{
	using namespace nihil::ucl;

	static_assert(std::same_as<double, real::contained_type>);
	REQUIRE(real::ucl_type == object_type::real);
	REQUIRE(static_cast<::ucl_type>(real::ucl_type) == UCL_FLOAT);

	static_assert(std::destructible<real>);
	static_assert(std::default_initializable<real>);
	static_assert(std::move_constructible<real>);
	static_assert(std::copy_constructible<real>);
	static_assert(std::equality_comparable<real>);
	static_assert(std::totally_ordered<real>);
	static_assert(std::swappable<real>);
}

TEST_CASE("ucl: real: constructor", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION ("default") {
		auto r = real();
		REQUIRE(r == 0);
	}

	SECTION ("with value") {
		auto r = real(42.1);
		REQUIRE_THAT(r.value(), Catch::Matchers::WithinRel(42.1));
	}
}

TEST_CASE("ucl: real: literal", "[ucl]")
{
	SECTION ("with namespace nihil::ucl::literals") {
		using namespace nihil::ucl::literals;

		auto r = 42.5_ucl;
		REQUIRE(r.type() == nihil::ucl::object_type::real);
		REQUIRE_THAT(r.value(), Catch::Matchers::WithinRel(42.5));
	}

	SECTION ("with namespace nihil::literals") {
		using namespace nihil::literals;

		auto r = 42.5_ucl;
		REQUIRE(r.type() == nihil::ucl::object_type::real);
		REQUIRE_THAT(r.value(), Catch::Matchers::WithinRel(42.5));
	}
}

TEST_CASE("ucl: real: construct from UCL object", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION ("ref, correct type") {
		auto *uobj = ::ucl_object_fromdouble(42);

		auto r = real(ref, uobj);
		REQUIRE(r == 42);

		::ucl_object_unref(uobj);
	}

	SECTION ("noref, correct type") {
		auto *uobj = ::ucl_object_fromdouble(42);

		auto r = real(noref, uobj);
		REQUIRE(r == 42);
	}

	SECTION ("ref, wrong type") {
		auto *uobj = ::ucl_object_fromint(42);

		REQUIRE_THROWS_AS(real(ref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}

	SECTION ("noref, wrong type") {
		auto *uobj = ::ucl_object_fromint(42);

		REQUIRE_THROWS_AS(real(noref, uobj), type_mismatch);

		::ucl_object_unref(uobj);
	}
}

TEST_CASE("ucl: real: make_real", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION ("default value") {
		auto i = make_real().value();
		REQUIRE(i == 0);
	}

	SECTION ("explicit value") {
		auto i = make_real(42).value();
		REQUIRE(i == 42);
	}
}

TEST_CASE("ucl: real: swap", "[ucl]")
{
	// do not add using namespace nihil::ucl

	auto r1 = nihil::ucl::real(1);
	auto r2 = nihil::ucl::real(2);

	swap(r1, r2);

	REQUIRE(r1 == 2.);
	REQUIRE(r2 == 1.);
}

TEST_CASE("ucl: real: value()", "[ucl]")
{
	using namespace nihil::ucl;

	auto r = 42.5_ucl;
	REQUIRE_THAT(r.value(), Catch::Matchers::WithinRel(42.5));
}

TEST_CASE("ucl: real: key()", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION ("parsed with key") {
		auto obj = parse("a_real = 42.5").value();
		auto r = object_cast<real>(obj["a_real"]).value();
		REQUIRE(r.key() == "a_real");
	}

	SECTION ("bare real, no key") {
		auto i = 42.5_ucl;
		REQUIRE(i.key().empty() == true);
	}
}

TEST_CASE("ucl: real: comparison", "[ucl]")
{
	using namespace nihil::ucl;

	auto i = nihil::ucl::real(42.5);

	SECTION ("operator==") {
		REQUIRE(i == 42.5);
		REQUIRE(i == 42.5_ucl);
	}

	SECTION ("operator!=") {
		REQUIRE(i != 1);
		REQUIRE(i != 1._ucl);
	}

	SECTION ("operator<") {
		REQUIRE(i < 43);
		REQUIRE(i < 43._ucl);
	}

	SECTION ("operator>") {
		REQUIRE(i > 1);
		REQUIRE(i > 1._ucl);
	}
}

TEST_CASE("ucl: real: parse", "[ucl]")
{
	using namespace nihil::ucl;

	auto obj = parse("value = 42.1").value();

	auto v = obj["value"];
	REQUIRE(v.key() == "value");
	REQUIRE_THAT(object_cast<real>(v).value().value(), Catch::Matchers::WithinRel(42.1));
}

TEST_CASE("ucl: real: parse and emit", "[ucl]")
{
	using namespace nihil::ucl;

	auto ucl = parse("real = 42.2").value();

	auto output = std::string();
	emit(ucl, emitter::configuration, std::back_inserter(output));

	REQUIRE(output == "real = 42.2;\n");
}

TEST_CASE("ucl: real: format", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION ("bare real") {
		auto str = std::format("{}", 42.5_ucl);
		REQUIRE(str == "42.5");
	}

	SECTION ("parsed real") {
		auto obj = parse("real = 42.5;").value();
		auto r = object_cast<real>(obj["real"]).value();

		auto str = std::format("{}", r);
		REQUIRE(str == "42.5");
	}

	SECTION ("with format string") {
		auto str = std::format("{:10.5f}", 42.5_ucl);
		REQUIRE(str == "  42.50000");
	}
}

TEST_CASE("ucl: real: print to ostream", "[ucl]")
{
	using namespace nihil::ucl;

	SECTION ("bare real") {
		auto strm = std::ostringstream();
		strm << 42.5_ucl;

		REQUIRE(strm.str() == "42.5");
	}

	SECTION ("parsed real") {
		auto obj = parse("real = 42.5;").value();
		auto i = object_cast<real>(obj["real"]).value();

		auto strm = std::ostringstream();
		strm << i;

		REQUIRE(strm.str() == "42.5");
	}
}
} // anonymous namespace
