/*
 * From https://github.com/mariusbancila/stduuid
 *
 * Copyright (c) 2017
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <algorithm>
#include <random>
#include <set>
#include <unordered_set>

#include <catch2/catch_test_macros.hpp>

namespace
{

// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0205r0.html
template <typename EngineT, std::size_t StateSize = EngineT::state_size>
void seed_rng(EngineT& engine)
{
	using engine_type = typename EngineT::result_type;
	using device_type = std::random_device::result_type;
	using seedseq_type = std::seed_seq::result_type;

	constexpr auto bytes_needed = StateSize * sizeof(engine_type);
	constexpr auto numbers_needed =
		(sizeof(device_type) < sizeof(seedseq_type))
		? (bytes_needed / sizeof(device_type))
		: (bytes_needed / sizeof(seedseq_type));

	auto numbers = std::array<device_type, numbers_needed>{};
	auto rnddev = std::random_device{};
	std::ranges::generate(numbers, std::ref(rnddev));

	auto seedseq = std::seed_seq(std::cbegin(numbers),
				     std::cend(numbers));
	engine.seed(seedseq);
}

} // anonymous namespace

import nihil;

using namespace nihil;

TEST_CASE("uuid: Test multiple default generators", "[uuid]")
{
	uuid id1;
	uuid id2;

	{
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 generator(seq);

		id1 = uuid_random_generator{ generator }();
		REQUIRE(!id1.is_nil());
		REQUIRE(id1.version() == uuid_version::random_number_based);
		REQUIRE(id1.variant() == uuid_variant::rfc);
	}

	{
		std::random_device rd;
		auto seed_data = std::array<int, std::mt19937::state_size> {};
		std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		std::mt19937 generator(seq);

		id2 = uuid_random_generator{ generator }();
		REQUIRE(!id2.is_nil());
		REQUIRE(id2.version() == uuid_version::random_number_based);
		REQUIRE(id2.variant() == uuid_variant::rfc);
	}

	REQUIRE(id1 != id2);
}

TEST_CASE("uuid: Test default generator", "[uuid]")
{
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 generator(seq);

	uuid const guid = uuid_random_generator{generator}();
	REQUIRE(!guid.is_nil());
	REQUIRE(guid.version() == uuid_version::random_number_based);
	REQUIRE(guid.variant() == uuid_variant::rfc);
}

TEST_CASE("uuid: Test random generator (conversion ctor w/ smart ptr)",
	  "[uuid]")
{
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 generator(seq);

	uuid_random_generator dgen(&generator);
	auto id1 = dgen();
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::random_number_based);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen();
	REQUIRE(!id2.is_nil());
	REQUIRE(id2.version() == uuid_version::random_number_based);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
}

TEST_CASE("uuid: Test random generator (conversion ctor w/ ptr)", "[uuid]")
{
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	auto generator = std::make_unique<std::mt19937>(seq);

	uuid_random_generator dgen(generator.get());
	auto id1 = dgen();
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::random_number_based);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen();
	REQUIRE(!id1.is_nil());
	REQUIRE(id2.version() == uuid_version::random_number_based);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
}

TEST_CASE("uuid: Test random generator (conversion ctor w/ ref)", "[uuid]")
{
	std::random_device rd;
	auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::mt19937 generator(seq);

	uuid_random_generator dgen(generator);
	auto id1 = dgen();
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::random_number_based);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen();
	REQUIRE(!id2.is_nil());
	REQUIRE(id2.version() == uuid_version::random_number_based);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
}

TEST_CASE("uuid: Test basic random generator (conversion ctor w/ ptr) "
	  "w/ ranlux48_base", "[uuid]")
{
	std::random_device rd;
	auto seed_data = std::array<int, 6> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::ranlux48_base generator(seq);

	basic_uuid_random_generator<std::ranlux48_base> dgen(&generator);
	auto id1 = dgen();
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::random_number_based);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen();
	REQUIRE(!id2.is_nil());
	REQUIRE(id2.version() == uuid_version::random_number_based);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
}

TEST_CASE("uuid: Test basic random generator (conversion ctor w/ smart ptr) "
	  "w/ ranlux48_base", "[uuid]")
{
	std::random_device rd;
	auto seed_data = std::array<int, 6> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	auto generator = std::make_unique<std::ranlux48_base>(seq);

	basic_uuid_random_generator<std::ranlux48_base> dgen(generator.get());
	auto id1 = dgen();
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::random_number_based);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen();
	REQUIRE(!id2.is_nil());
	REQUIRE(id2.version() == uuid_version::random_number_based);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
}

TEST_CASE("uuid: Test basic random generator (conversion ctor w/ ref) "
	  "w/ ranlux48_base", "[uuid]")
{
	std::random_device rd;
	auto seed_data = std::array<int, 6> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	std::ranlux48_base generator(seq);

	basic_uuid_random_generator<std::ranlux48_base> dgen(generator);
	auto id1 = dgen();
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::random_number_based);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen();
	REQUIRE(!id2.is_nil());
	REQUIRE(id2.version() == uuid_version::random_number_based);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
}

TEST_CASE("uuid: Test namespaces", "[uuid]")
{
	REQUIRE(uuid_namespace_dns == uuid::from_string("6ba7b810-9dad-11d1-80b4-00c04fd430c8"));
	REQUIRE(uuid_namespace_url == uuid::from_string("6ba7b811-9dad-11d1-80b4-00c04fd430c8"));
	REQUIRE(uuid_namespace_oid == uuid::from_string("6ba7b812-9dad-11d1-80b4-00c04fd430c8"));
	REQUIRE(uuid_namespace_x500 == uuid::from_string("6ba7b814-9dad-11d1-80b4-00c04fd430c8"));
}

TEST_CASE("uuid: Test name generator (char*)", "[uuid]")
{
	uuid_name_generator dgen(uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value());

	auto id1 = dgen("john");
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::name_based_sha1);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen("jane");
	REQUIRE(!id2.is_nil());
	REQUIRE(id2.version() == uuid_version::name_based_sha1);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	auto id3 = dgen("jane");
	REQUIRE(!id3.is_nil());
	REQUIRE(id3.version() == uuid_version::name_based_sha1);
	REQUIRE(id3.variant() == uuid_variant::rfc);

	auto id4 = dgen(L"jane");
	REQUIRE(!id4.is_nil());
	REQUIRE(id4.version() == uuid_version::name_based_sha1);
	REQUIRE(id4.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
	REQUIRE(id2 == id3);
	REQUIRE(id3 != id4);
}

TEST_CASE("uuid: Test name generator (std::string)", "[uuid]")
{
	using namespace std::string_literals;

	uuid_name_generator dgen(uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value());
	auto id1 = dgen("john"s);
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::name_based_sha1);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen("jane"s);
	REQUIRE(!id2.is_nil());
	REQUIRE(id2.version() == uuid_version::name_based_sha1);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	auto id3 = dgen("jane"s);
	REQUIRE(!id3.is_nil());
	REQUIRE(id3.version() == uuid_version::name_based_sha1);
	REQUIRE(id3.variant() == uuid_variant::rfc);

	auto id4 = dgen(L"jane"s);
	REQUIRE(!id4.is_nil());
	REQUIRE(id4.version() == uuid_version::name_based_sha1);
	REQUIRE(id4.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
	REQUIRE(id2 == id3);
	REQUIRE(id3 != id4);
}

TEST_CASE("uuid: Test name generator (std::string_view)", "[uuid]")
{
	using namespace std::string_view_literals;

	uuid_name_generator dgen(uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value());
	auto id1 = dgen("john"sv);
	REQUIRE(!id1.is_nil());
	REQUIRE(id1.version() == uuid_version::name_based_sha1);
	REQUIRE(id1.variant() == uuid_variant::rfc);

	auto id2 = dgen("jane"sv);
	REQUIRE(!id2.is_nil());
	REQUIRE(id2.version() == uuid_version::name_based_sha1);
	REQUIRE(id2.variant() == uuid_variant::rfc);

	auto id3 = dgen("jane"sv);
	REQUIRE(!id3.is_nil());
	REQUIRE(id3.version() == uuid_version::name_based_sha1);
	REQUIRE(id3.variant() == uuid_variant::rfc);

	auto id4 = dgen(L"jane"sv);
	REQUIRE(!id4.is_nil());
	REQUIRE(id4.version() == uuid_version::name_based_sha1);
	REQUIRE(id4.variant() == uuid_variant::rfc);

	REQUIRE(id1 != id2);
	REQUIRE(id2 == id3);
	REQUIRE(id3 != id4);
}

TEST_CASE("uuid: Test name generator equality (char const*, std::string, "
	  "std::string_view)", "[uuid]")
{
	using namespace std::literals;

	uuid_name_generator dgen(uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value());
	auto id1 = dgen("john");
	auto id2 = dgen("john"s);
	auto id3 = dgen("john"sv);

	REQUIRE(id1 == id2);
	REQUIRE(id2 == id3);
}

TEST_CASE("uuid: Test default constructor", "[uuid]")
{
	auto empty = uuid();
	REQUIRE(empty.is_nil());
}

TEST_CASE("uuid: Test string conversion", "[uuid]")
{
	auto empty = uuid();
	REQUIRE(to_string(empty) ==
		"00000000-0000-0000-0000-000000000000");
	REQUIRE(to_string<wchar_t>(empty) ==
		L"00000000-0000-0000-0000-000000000000");
}

TEST_CASE("uuid: Test is_valid_uuid(char*)", "[uuid]")
{
	REQUIRE(uuid::is_valid_uuid(
			"47183823-2574-4bfd-b411-99ed177d3e43"));
	REQUIRE(uuid::is_valid_uuid(
			"{47183823-2574-4bfd-b411-99ed177d3e43}"));
	REQUIRE(uuid::is_valid_uuid(
			L"47183823-2574-4bfd-b411-99ed177d3e43"));
	REQUIRE(uuid::is_valid_uuid(
			L"{47183823-2574-4bfd-b411-99ed177d3e43}"));
	REQUIRE(uuid::is_valid_uuid(
			"00000000-0000-0000-0000-000000000000"));
	REQUIRE(uuid::is_valid_uuid(
			"{00000000-0000-0000-0000-000000000000}"));
	REQUIRE(uuid::is_valid_uuid(
			L"00000000-0000-0000-0000-000000000000"));
	REQUIRE(uuid::is_valid_uuid(
			L"{00000000-0000-0000-0000-000000000000}"));
}

TEST_CASE("uuid: Test is_valid_uuid(basic_string)", "[uuid]")
{
	using namespace std::string_literals;

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e43"s;
		REQUIRE(uuid::is_valid_uuid(str));
	}

	{
		auto str = "{47183823-2574-4bfd-b411-99ed177d3e43}"s;
		REQUIRE(uuid::is_valid_uuid(str));
	}

	{
		auto str = L"47183823-2574-4bfd-b411-99ed177d3e43"s;
		REQUIRE(uuid::is_valid_uuid(str));
	}

	{
		auto str = L"{47183823-2574-4bfd-b411-99ed177d3e43}"s;
		REQUIRE(uuid::is_valid_uuid(str));
	}

	{
		auto str = "00000000-0000-0000-0000-000000000000"s;
		REQUIRE(uuid::is_valid_uuid(str));
	}

	{
		auto str = "{00000000-0000-0000-0000-000000000000}"s;
		REQUIRE(uuid::is_valid_uuid(str));
	}

	{
		auto str = L"00000000-0000-0000-0000-000000000000"s;
		REQUIRE(uuid::is_valid_uuid(str));
	}

	{
		auto str = L"{00000000-0000-0000-0000-000000000000}"s;
		REQUIRE(uuid::is_valid_uuid(str));
	}
}

TEST_CASE("uuid: Test is_valid_uuid(basic_string_view)", "[uuid]")
{
	using namespace std::string_view_literals;

	REQUIRE(uuid::is_valid_uuid(
			"47183823-2574-4bfd-b411-99ed177d3e43"sv));
	REQUIRE(uuid::is_valid_uuid(
			"{47183823-2574-4bfd-b411-99ed177d3e43}"sv));
	REQUIRE(uuid::is_valid_uuid(
			L"47183823-2574-4bfd-b411-99ed177d3e43"sv));
	REQUIRE(uuid::is_valid_uuid(
			L"{47183823-2574-4bfd-b411-99ed177d3e43}"sv));
	REQUIRE(uuid::is_valid_uuid(
			"00000000-0000-0000-0000-000000000000"sv));
	REQUIRE(uuid::is_valid_uuid(
			"{00000000-0000-0000-0000-000000000000}"sv));
	REQUIRE(uuid::is_valid_uuid(
			L"00000000-0000-0000-0000-000000000000"sv));
	REQUIRE(uuid::is_valid_uuid(
			L"{00000000-0000-0000-0000-000000000000}"sv));
}

TEST_CASE("uuid: Test is_valid_uuid(char*) invalid format", "[uuid]")
{
	REQUIRE(!uuid::is_valid_uuid(""));
	REQUIRE(!uuid::is_valid_uuid("{}"));
	REQUIRE(!uuid::is_valid_uuid(
			"47183823-2574-4bfd-b411-99ed177d3e4"));
	REQUIRE(!uuid::is_valid_uuid(
			"47183823-2574-4bfd-b411-99ed177d3e430"));
	REQUIRE(!uuid::is_valid_uuid(
			"{47183823-2574-4bfd-b411-99ed177d3e43"));
	REQUIRE(!uuid::is_valid_uuid(
			"47183823-2574-4bfd-b411-99ed177d3e43}"));
}

TEST_CASE("uuid: Test is_valid_uuid(basic_string) invalid format", "[uuid]")
{
	using namespace std::string_literals;

	{
		auto str = ""s;
		REQUIRE(!uuid::is_valid_uuid(str));
	}

	{
		auto str = "{}"s;
		REQUIRE(!uuid::is_valid_uuid(str));
	}

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e4"s;
		REQUIRE(!uuid::is_valid_uuid(str));
	}

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e430"s;
		REQUIRE(!uuid::is_valid_uuid(str));
	}

	{
		auto str = "{47183823-2574-4bfd-b411-99ed177d3e43"s;
		REQUIRE(!uuid::is_valid_uuid(str));
	}

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e43}"s;
		REQUIRE(!uuid::is_valid_uuid(str));
	}
}

TEST_CASE("uuid: Test is_valid_uuid(basic_string_view) invalid format",
	  "[uuid]")
{
	using namespace std::string_view_literals;

	REQUIRE(!uuid::is_valid_uuid(""sv));
	REQUIRE(!uuid::is_valid_uuid("{}"sv));
	REQUIRE(!uuid::is_valid_uuid(
			"47183823-2574-4bfd-b411-99ed177d3e4"sv));
	REQUIRE(!uuid::is_valid_uuid(
			"47183823-2574-4bfd-b411-99ed177d3e430"sv));
	REQUIRE(!uuid::is_valid_uuid(
			"{47183823-2574-4bfd-b411-99ed177d3e43"sv));
	REQUIRE(!uuid::is_valid_uuid(
			"47183823-2574-4bfd-b411-99ed177d3e43}"sv));
}

TEST_CASE("uuid: Test from_string(char*)", "[uuid]")
{
	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e43";
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string(guid) == str);
	}

	{
		auto str = "{47183823-2574-4bfd-b411-99ed177d3e43}";
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string(guid) == "47183823-2574-4bfd-b411-99ed177d3e43");
	}

	{
		auto guid = uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value();
		REQUIRE(to_string(guid) == "47183823-2574-4bfd-b411-99ed177d3e43");
		REQUIRE(to_string<wchar_t>(guid) == L"47183823-2574-4bfd-b411-99ed177d3e43");
	}

	{
		auto str = L"47183823-2574-4bfd-b411-99ed177d3e43";
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string<wchar_t>(guid) == str);
	}

	{
		auto str = "4718382325744bfdb41199ed177d3e43";
		REQUIRE_NOTHROW(uuid::from_string(str));
		REQUIRE(uuid::from_string(str).has_value());
	}

	{
		auto str = "00000000-0000-0000-0000-000000000000";
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = "{00000000-0000-0000-0000-000000000000}";
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = L"00000000-0000-0000-0000-000000000000";
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = L"{00000000-0000-0000-0000-000000000000}";
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}
}

TEST_CASE("uuid: Test from_string(basic_string)", "[uuid]")
{
	using namespace std::string_literals;

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e43"s;
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string(guid) == str);
	}

	{
		auto str = "{47183823-2574-4bfd-b411-99ed177d3e43}"s;
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string(guid) == "47183823-2574-4bfd-b411-99ed177d3e43");
	}

	{
		auto guid = uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43"s).value();
		REQUIRE(to_string(guid) == "47183823-2574-4bfd-b411-99ed177d3e43");
		REQUIRE(to_string<wchar_t>(guid) == L"47183823-2574-4bfd-b411-99ed177d3e43");
	}

	{
		auto str = L"47183823-2574-4bfd-b411-99ed177d3e43"s;
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string<wchar_t>(guid) == str);
	}

	{
		auto str = "4718382325744bfdb41199ed177d3e43"s;
		REQUIRE_NOTHROW(uuid::from_string(str));
		REQUIRE(uuid::from_string(str).has_value());
	}

	{
		auto str = "00000000-0000-0000-0000-000000000000"s;
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = "{00000000-0000-0000-0000-000000000000}"s;
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = L"00000000-0000-0000-0000-000000000000"s;
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = L"{00000000-0000-0000-0000-000000000000}"s;
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}
}

TEST_CASE("uuid: Test from_string(basic_string_view)", "[uuid]")
{
	using namespace std::string_view_literals;

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e43"sv;
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string(guid) == str);
	}

	{
		auto str = "{47183823-2574-4bfd-b411-99ed177d3e43}"sv;
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string(guid) == "47183823-2574-4bfd-b411-99ed177d3e43");
	}

	{
		auto guid = uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43"sv).value();
		REQUIRE(to_string(guid) == "47183823-2574-4bfd-b411-99ed177d3e43");
		REQUIRE(to_string<wchar_t>(guid) == L"47183823-2574-4bfd-b411-99ed177d3e43");
	}

	{
		auto str = L"47183823-2574-4bfd-b411-99ed177d3e43"sv;
		auto guid = uuid::from_string(str).value();
		REQUIRE(to_string<wchar_t>(guid) == str);
	}

	{
		auto str = "4718382325744bfdb41199ed177d3e43"sv;
		REQUIRE_NOTHROW(uuid::from_string(str));
		REQUIRE(uuid::from_string(str).has_value());
	}

	{
		auto str = "00000000-0000-0000-0000-000000000000"sv;
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = "{00000000-0000-0000-0000-000000000000}"sv;
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = L"00000000-0000-0000-0000-000000000000"sv;
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}

	{
		auto str = L"{00000000-0000-0000-0000-000000000000}"sv;
		auto guid = uuid::from_string(str).value();
		REQUIRE(guid.is_nil());
	}
}

TEST_CASE("uuid: Test constexpr from_string", "[uuid]")
{
	constexpr uuid value = uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value();
	static_assert(!value.is_nil());
	static_assert(value.variant() == uuid_variant::rfc);
	static_assert(value.version() != uuid_version::none);
}

TEST_CASE("uuid: Test from_string(char*) invalid format", "[uuid]")
{
	REQUIRE(!uuid::from_string("").has_value());
	REQUIRE(!uuid::from_string("{}").has_value());
	REQUIRE(!uuid::from_string(
			"47183823-2574-4bfd-b411-99ed177d3e4").has_value());
	REQUIRE(!uuid::from_string(
			"47183823-2574-4bfd-b411-99ed177d3e430").has_value());
	REQUIRE(!uuid::from_string(
			"{47183823-2574-4bfd-b411-99ed177d3e43").has_value());
	REQUIRE(!uuid::from_string(
			"47183823-2574-4bfd-b411-99ed177d3e43}").has_value());
}

TEST_CASE("uuid: Test from_string(basic_string) invalid format", "[uuid]")
{
	using namespace std::string_literals;

	{
		auto str = ""s;
		REQUIRE(!uuid::from_string(str).has_value());
	}

	{
		auto str = "{}"s;
		REQUIRE(!uuid::from_string(str).has_value());
	}

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e4"s;
		REQUIRE(!uuid::from_string(str).has_value());
	}

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e430"s;
		REQUIRE(!uuid::from_string(str).has_value());
	}

	{
		auto str = "{47183823-2574-4bfd-b411-99ed177d3e43"s;
		REQUIRE(!uuid::from_string(str).has_value());
	}

	{
		auto str = "47183823-2574-4bfd-b411-99ed177d3e43}"s;
		REQUIRE(!uuid::from_string(str).has_value());
	}
}

TEST_CASE("uuid: Test from_string(basic_string_view) invalid format", "[uuid]")
{
	using namespace std::string_view_literals;

	REQUIRE(!uuid::from_string(""sv).has_value());
	REQUIRE(!uuid::from_string("{}"sv).has_value());
	REQUIRE(!uuid::from_string(
			"47183823-2574-4bfd-b411-99ed177d3e4"sv).has_value());
	REQUIRE(!uuid::from_string(
			"47183823-2574-4bfd-b411-99ed177d3e430"sv).has_value());
	REQUIRE(!uuid::from_string(
			"{47183823-2574-4bfd-b411-99ed177d3e43"sv).has_value());
	REQUIRE(!uuid::from_string(
			"47183823-2574-4bfd-b411-99ed177d3e43}"sv).has_value());
}

TEST_CASE("uuid: Test iterators constructor", "[uuid]")
{
	using namespace std::string_literals;

	{
		std::array<uuid::value_type, 16> arr{{
			0x47, 0x18, 0x38, 0x23,
			0x25, 0x74,
			0x4b, 0xfd,
			0xb4, 0x11,
			0x99, 0xed, 0x17, 0x7d, 0x3e, 0x43
		}};

		uuid guid(std::begin(arr), std::end(arr));
		REQUIRE(to_string(guid) ==
			"47183823-2574-4bfd-b411-99ed177d3e43"s);
	}

	{
		uuid::value_type arr[16] = {
			0x47, 0x18, 0x38, 0x23,
			0x25, 0x74,
			0x4b, 0xfd,
			0xb4, 0x11,
			0x99, 0xed, 0x17, 0x7d, 0x3e, 0x43
		};

		uuid guid(std::begin(arr), std::end(arr));
		REQUIRE(to_string(guid) ==
			"47183823-2574-4bfd-b411-99ed177d3e43"s);
	}
}

TEST_CASE("uuid: Test array constructors", "[uuid]")
{
	using namespace std::string_literals;

	{
		uuid guid{{
			0x47, 0x18, 0x38, 0x23,
			0x25, 0x74,
			0x4b, 0xfd,
			0xb4, 0x11,
			0x99, 0xed, 0x17, 0x7d, 0x3e, 0x43
		}};

		REQUIRE(to_string(guid) ==
			"47183823-2574-4bfd-b411-99ed177d3e43"s);
	}

	{
		std::array<uuid::value_type, 16> arr{{
			0x47, 0x18, 0x38, 0x23,
			0x25, 0x74,
			0x4b, 0xfd,
			0xb4, 0x11,
			0x99, 0xed, 0x17, 0x7d, 0x3e, 0x43
		}};

		uuid guid(arr);
		REQUIRE(to_string(guid) ==
			"47183823-2574-4bfd-b411-99ed177d3e43"s);
	}

	{
		uuid::value_type arr[16] {
			0x47, 0x18, 0x38, 0x23,
			0x25, 0x74,
			0x4b, 0xfd,
			0xb4, 0x11,
			0x99, 0xed, 0x17, 0x7d, 0x3e, 0x43
		};

		uuid guid(arr);
		REQUIRE(to_string(guid) ==
			"47183823-2574-4bfd-b411-99ed177d3e43"s);
	}
}

TEST_CASE("uuid: Test equality", "[uuid]")
{
	uuid empty;

	auto engine = uuid_random_generator::engine_type{};
	seed_rng(engine);
	uuid guid = uuid_random_generator{engine}();

	REQUIRE(empty == empty);
	REQUIRE(guid == guid);
	REQUIRE(empty != guid);
}

TEST_CASE("Test comparison", "[uuid]")
{
	auto empty = uuid{};

	auto engine = uuid_random_generator::engine_type{};
	seed_rng(engine);

	uuid_random_generator gen{engine};
	auto id = gen();

	REQUIRE(empty < id);

	std::set<uuid> ids{
		uuid{},
		gen(),
		gen(),
		gen(),
		gen()
	};

	REQUIRE(ids.size() == 5);
	REQUIRE(ids.find(uuid{}) != ids.end());
}

TEST_CASE("uuid: Test hashing", "[uuid]")
{
	using namespace std::string_literals;

	auto str = "47183823-2574-4bfd-b411-99ed177d3e43"s;
	auto guid = uuid::from_string(str).value();

	auto h1 = std::hash<std::string>{};
	auto h2 = std::hash<uuid>{};
	REQUIRE(h1(str) != h2(guid));

	auto engine = uuid_random_generator::engine_type{};
	seed_rng(engine);
	uuid_random_generator gen{ engine };

	std::unordered_set<uuid> ids{
		uuid{},
		gen(),
		gen(),
		gen(),
		gen()
	};

	REQUIRE(ids.size() == 5);
	REQUIRE(ids.find(uuid{}) != ids.end());
}

TEST_CASE("uuid: Test swap", "[uuid]")
{
	uuid empty;

	auto engine = uuid_random_generator::engine_type{};
	seed_rng(engine);
	uuid guid = uuid_random_generator{engine}();

	REQUIRE(empty.is_nil());
	REQUIRE(!guid.is_nil());

	std::swap(empty, guid);

	REQUIRE(!empty.is_nil());
	REQUIRE(guid.is_nil());

	empty.swap(guid);

	REQUIRE(empty.is_nil());
	REQUIRE(!guid.is_nil());
}

TEST_CASE("uuid: Test constexpr", "[uuid]")
{
	constexpr uuid empty;
	static_assert(empty.is_nil());
	static_assert(empty.variant() == uuid_variant::ncs);
	static_assert(empty.version() == uuid_version::none);
}

TEST_CASE("uuid: Test size", "[uuid]")
{
   REQUIRE(sizeof(uuid) == 16);
}

TEST_CASE("uuid: Test assignment", "[uuid]")
{
	auto id1 = uuid::from_string("47183823-2574-4bfd-b411-99ed177d3e43").value();
	auto id2 = id1;
	REQUIRE(id1 == id2);

	id1 = uuid::from_string("{fea43102-064f-4444-adc2-02cec42623f8}").value();
	REQUIRE(id1 != id2);

	auto id3 = std::move(id2);
	REQUIRE(to_string(id3) == "47183823-2574-4bfd-b411-99ed177d3e43");
}

TEST_CASE("uuid: Test trivial", "[uuid]")
{
	REQUIRE(std::is_trivially_copyable_v<uuid>);
}

TEST_CASE("uuid: Test as_bytes", "[uuid]")
{
	std::array<uuid::value_type, 16> arr{{
		0x47, 0x18, 0x38, 0x23,
		0x25, 0x74,
		0x4b, 0xfd,
		0xb4, 0x11,
		0x99, 0xed, 0x17, 0x7d, 0x3e, 0x43
	}};

	{
		uuid id{ arr };
		REQUIRE(!id.is_nil());

		auto view = id.as_bytes();
		REQUIRE(memcmp(view.data(), arr.data(), arr.size()) == 0);
	}

	{
		const uuid id{ arr };
		REQUIRE(!id.is_nil());

		auto view = id.as_bytes();
		REQUIRE(memcmp(view.data(), arr.data(), arr.size()) == 0);
	}
}

