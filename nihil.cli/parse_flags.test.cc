// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.cli;
import nihil.core;

namespace {

inline auto constexpr *test_tags = "[nihil][nihil.cli][nihil.cli_parse_flags]";

SCENARIO("Parsing several flags with flag_parser", test_tags)
{
	using namespace std::literals;
	using nihil::cli::option;
	using nihil::cli::options;

	GIVEN ("A flag definition") {
		struct test_flags
		{
			bool        a_flag{};
			bool        b_flag{};
			std::string s_flag;
		};

		auto flags = options<test_flags>(option('a', &test_flags::a_flag),
		                                 option('b', &test_flags::b_flag),
		                                 option('s', "sflag", &test_flags::s_flag));

		WHEN ("We parse '-a'") {
			auto ret = posix_parse(flags, std::array{"-a"sv}).value();

			THEN ("a_flag is set") {
				REQUIRE(ret.a_flag);
			}

			AND_THEN ("The other flags are not") {
				REQUIRE(!ret.b_flag);
				REQUIRE(ret.s_flag.empty());
			}
		}

		WHEN ("We parse '-ab'") {
			auto ret = posix_parse(flags, std::array{"-ab"sv}).value();

			THEN ("a_flag and b_flag are set") {
				REQUIRE(ret.a_flag);
				REQUIRE(ret.b_flag);
			}

			AND_THEN ("s_flag is not set") {
				REQUIRE(ret.s_flag.empty());
			}
		}

		WHEN ("We parse '-sarg'") {
			auto ret = posix_parse(flags, std::array{"-sarg"sv}).value();

			THEN ("s_flag is set to 'arg'") {
				REQUIRE(ret.s_flag == "arg");
			}

			AND_THEN ("The other flags are not set") {
				REQUIRE(!ret.a_flag);
				REQUIRE(!ret.b_flag);
			}
		}

		WHEN ("We parse '-s arg'") {
			auto ret = posix_parse(flags, std::array{"-s"sv, "arg"sv}).value();

			THEN ("s_flag is set to 'arg'") {
				REQUIRE(ret.s_flag == "arg");
			}

			AND_THEN ("The other flags are not set") {
				REQUIRE(!ret.a_flag);
				REQUIRE(!ret.b_flag);
			}
		}

		WHEN ("We parse '-a -sarg'") {
			auto ret = posix_parse(flags, std::array{"-a"sv, "-sarg"sv}).value();

			THEN ("a_flag is set and s_flag is set to 'arg'") {
				REQUIRE(ret.a_flag);
				REQUIRE(ret.s_flag == "arg");
			}

			AND_THEN ("b_flag is not set") {
				REQUIRE(!ret.b_flag);
			}
		}

		WHEN ("We parse '-asarg'") {
			auto ret = posix_parse(flags, std::array{"-asarg"sv}).value();

			THEN ("a_flag is set and s_flag is set to 'arg'") {
				REQUIRE(ret.a_flag);
				REQUIRE(ret.s_flag == "arg");
			}

			AND_THEN ("b_flag is not set") {
				REQUIRE(!ret.b_flag);
			}
		}
	}
}

SCENARIO("Calling flag_parser with incorrect options", test_tags)
{
	using namespace std::literals;
	using nihil::cli::option;
	using nihil::cli::options;

	GIVEN ("A flag definition") {
		struct test_flags
		{
			bool        a_flag{};
			std::string s_flag;
		};

		auto flags = options<test_flags>(option('a', &test_flags::a_flag),
		                                 option('s', "sflag", &test_flags::s_flag));

		WHEN ("We parse '-x'") {
			auto ret = posix_parse(flags, std::array{"-x"sv});

			THEN ("An unknown argument error is returned") {
				REQUIRE(!ret);
				REQUIRE(std::format("{}", ret.error()) == "-x: unknown option");
			}
		}

		WHEN ("We parse '-s'") {
			auto ret = posix_parse(flags, std::array{"-s"sv});

			THEN ("A missing argument error is returned") {
				REQUIRE(!ret);
				REQUIRE(std::format("{}", ret.error()) == "-s: argument required");
			}
		}
	}
}

SCENARIO("Terminating the option list with '--'", test_tags)
{
	using namespace std::literals;
	using nihil::cli::option;
	using nihil::cli::options;

	GIVEN ("A flag parser with -a and -b options") {
		struct test_flags
		{
			bool a_flag{};
			bool b_flag{};
			std::string arg1;
		};

		auto flags = options<test_flags>(option('a', &test_flags::a_flag),
		                                 option('b', &test_flags::b_flag),
		                                 option("arg1", &test_flags::arg1));

		WHEN ("We parse '-a -- -b'") {
			auto ret = posix_parse(flags, std::array{"-a"sv, "--"sv, "-b"sv});

			THEN ("The parse was successful") {
				if (!ret) {
					INFO(ret.error());
					FAIL();
				}
			}
			AND_THEN ("a_flag is set and b_flag is not") {
				REQUIRE(ret->a_flag == true);
				REQUIRE(ret->b_flag == false);
			}
			AND_THEN ("arg1 is set") {
				REQUIRE(ret->arg1 == "-b");
			}
		}
	}
}

SCENARIO("Creating a posix_usage() string", test_tags)
{
	using namespace std::literals;
	using nihil::cli::option;
	using nihil::cli::options;

	GIVEN ("A flag parser with only boolean options") {
		struct test_flags
		{
			bool a_flag{};
			bool b_flag{};
		};

		auto flags = options<test_flags>(option('a', &test_flags::a_flag),
		                                 option('b', &test_flags::b_flag));

		WHEN ("We call posix_usage()") {
			auto usage = posix_usage(flags);

			THEN ("The option list is '[-ab]'") {
				REQUIRE(usage == "[-ab]");
			}
		}
	}

	GIVEN ("A flag parser with boolean and string options") {
		struct test_flags
		{
			bool a_flag{};
			bool b_flag{};
			std::string c_flag;
			std::optional<std::string> d_flag;
		};

		auto flags = options<test_flags>(option('a', &test_flags::a_flag),
						 option('b', &test_flags::b_flag),
						 option('c', "cflag", &test_flags::c_flag),
						 option('d', "dflag", &test_flags::d_flag));

		WHEN ("We call posix_usage()") {
			auto usage = posix_usage(flags);

			THEN ("The option list is '[-ab] -c <cflag> [-d <dflag>]'") {
				REQUIRE(usage == "[-ab] -c <cflag> [-d <dflag>]");
			}
		}
	}

	GIVEN ("A flag parser with only string options") {
		struct test_flags
		{
			std::string c_flag;
			std::string d_flag;
		};

		auto flags = options<test_flags>(option('c', "cflag", &test_flags::c_flag),
						 option('d', "dflag", &test_flags::d_flag));

		WHEN ("We call posix_usage()") {
			auto usage = posix_usage(flags);

			THEN ("The option list is '-c <cflag> -d <dflag>'") {
				REQUIRE(usage == "-c <cflag> -d <dflag>");
			}
		}
	}
}

SCENARIO("Calling posix_parse() with an (argc, argv) vector", test_tags)
{
	using namespace std::literals;
	using nihil::cli::option;
	using nihil::cli::options;

	GIVEN ("A flag definition")
	{
		struct test_flags
		{
			bool        a_flag{};
			bool        b_flag{};
			std::string s_flag;
		};

		auto flags = options<test_flags>(option('a', &test_flags::a_flag),
						 option('b', &test_flags::b_flag),
						 option('s', "sflag", &test_flags::s_flag));

		WHEN ("We parse '-a -s foo'") {
			// const_cast the args to char** to match the actual signature of main().
			auto argv = std::array{"progname", "-a", "-s", "foo",
			                       static_cast<char const *>(nullptr)};
			auto ret = posix_parse(flags, argv.size() - 1,
			                       const_cast<char **>(argv.data()))
			                   .value();

			THEN ("a_flag and s_flag are set") {
				REQUIRE(ret.a_flag);
				REQUIRE(ret.s_flag == "foo");
			}

			AND_THEN ("b_flag is not set") {
				REQUIRE(!ret.b_flag);
			}
		}
	}
}

SCENARIO("Calling posix_parse() with required arguments", test_tags)
{
	using namespace std::literals;
	using nihil::cli::option;
	using nihil::cli::options;

	GIVEN ("An options parser with two required arguments")
	{
		struct test_flags
		{
			std::string arg1;
			std::string arg2;
		};

		auto flags = options<test_flags>(option("arg1", &test_flags::arg1),
		                                 option("arg2", &test_flags::arg2));

		WHEN ("We parse 'foo bar'") {
			auto ret = posix_parse(flags, std::array{"foo", "bar"});

			THEN ("The parse was successful") {
				if (!ret) {
					INFO(ret.error());
					FAIL();
				}
			}
			AND_THEN("Both arg1 and arg2 are set correctly")
			{
				REQUIRE(ret->arg1 == "foo");
				REQUIRE(ret->arg2 == "bar");
			}
		}

		WHEN ("We parse '-- -foo -bar'") {
			auto ret = posix_parse(flags, std::array{"--", "-foo", "-bar"});

			THEN ("The parse was successful") {
				if (!ret) {
					INFO(ret.error());
					FAIL();
				}
			}
			AND_THEN("Both arg1 and arg2 are set correctly")
			{
				REQUIRE(ret->arg1 == "-foo");
				REQUIRE(ret->arg2 == "-bar");
			}
		}
	}
}

SCENARIO("Calling posix_parse() with required and optional arguments", test_tags)
{
	using namespace std::literals;
	using nihil::cli::option;
	using nihil::cli::options;

	GIVEN ("An options parser with a required and an optional argument")
	{
		struct test_flags
		{
			std::string arg1;
			std::optional<std::string> arg2;
		};

		auto flags = options<test_flags>(option("arg1", &test_flags::arg1),
						 option("arg2", &test_flags::arg2));

		WHEN ("We parse 'foo bar'") {
			auto ret = posix_parse(flags, std::array{"foo", "bar"});

			THEN ("The parse was successful") {
				if (!ret) {
					INFO(ret.error());
					FAIL();
				}
			}
			AND_THEN("Both arg1 and arg2 are set correctly")
			{
				REQUIRE(ret->arg1 == "foo");
				REQUIRE(ret->arg2 == "bar");
			}
		}

		WHEN ("We parse 'foo'") {
			auto ret = posix_parse(flags, std::array{"foo"});

			THEN ("The parse was successful") {
				if (!ret) {
					INFO(ret.error());
					FAIL();
				}
			}
			AND_THEN("arg1 is set and arg2 is not")
			{
				REQUIRE(ret->arg1 == "foo");
				REQUIRE(ret->arg2.has_value() == false);
			}
		}
	}
}

SCENARIO("Parsing integers with posix_parse()", test_tags)
{
	using namespace std::literals;
	using nihil::cli::option;
	using nihil::cli::options;

	GIVEN ("An options parser with an int16_t flag")
	{
		struct test_flags
		{
			std::int16_t iflag{};
		};

		auto flags = options<test_flags>(option('i', "iflag", &test_flags::iflag));

		WHEN ("The value is a small positive integer") {
			auto ret = posix_parse(flags, std::array{"-i", "30000"});

			THEN ("The parse was successful") {
				if (!ret) {
					INFO(ret.error());
					FAIL();
				}
			}
			AND_THEN("iflag is set correctly")
			{
				REQUIRE(ret->iflag == 30000);
			}
		}

		WHEN ("The value is a small negative integer") {
			auto ret = posix_parse(flags, std::array{"-i", "-30000"});

			THEN ("The parse was successful") {
				if (!ret) {
					INFO(ret.error());
					FAIL();
				}
			}
			AND_THEN("iflag is set correctly")
			{
				REQUIRE(ret->iflag == -30000);
			}
		}

		WHEN ("The value is a large positive integer") {
			auto ret = posix_parse(flags, std::array{"-i", "40000"});

			THEN ("The parse failed") {
				REQUIRE(!ret);
				REQUIRE(std::format("{}", ret.error()) == "-i: value too large");
			}
		}

		WHEN ("The value is a large negative integer") {
			auto ret = posix_parse(flags, std::array{"-i", "-40000"});

			THEN ("The parse failed") {
				REQUIRE(!ret);
				REQUIRE(std::format("{}", ret.error()) == "-i: value too small");
			}
		}

		WHEN ("The value is not a number") {
			auto ret = posix_parse(flags, std::array{"-i", "foo"});

			THEN ("The parse failed") {
				REQUIRE(!ret);
				REQUIRE(std::format("{}", ret.error()) ==
				        "-i: expected an integer");
			}
		}
	}

	GIVEN ("An options parser with a uint16_t flag")
	{
		struct test_flags
		{
			std::uint16_t iflag{};
		};

		auto flags = options<test_flags>(option('i', "iflag", &test_flags::iflag));

		WHEN ("The value is a positive integer") {
			auto ret = posix_parse(flags, std::array{"-i", "60000"});

			THEN ("The parse was successful") {
				if (!ret) {
					INFO(ret.error());
					FAIL();
				}
			}
			AND_THEN("iflag is set correctly")
			{
				REQUIRE(ret->iflag == 60000);
			}
		}

		WHEN ("The value is a large positive integer") {
			auto ret = posix_parse(flags, std::array{"-i", "80000"});

			THEN ("The parse failed") {
				REQUIRE(!ret);
				REQUIRE(std::format("{}", ret.error()) == "-i: value too large");
			}
		}

		WHEN ("The value is a negative integer") {
			auto ret = posix_parse(flags, std::array{"-i", "-1"});

			THEN ("The parse failed") {
				REQUIRE(!ret);
				REQUIRE(std::format("{}", ret.error()) ==
				        "-i: expected a non-negative integer");
			}
		}

		WHEN ("The value is not a number") {
			auto ret = posix_parse(flags, std::array{"-i", "foo"});

			THEN ("The parse failed") {
				REQUIRE(!ret);
				REQUIRE(std::format("{}", ret.error()) ==
					"-i: expected an integer");
			}
		}
	}
}

} // anonymous namespace
