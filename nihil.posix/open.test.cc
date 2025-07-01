// This source code is released into the public domain.

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.error;
import nihil.posix;

namespace {

SCENARIO("nihil::open() can open an existing file")
{
	GIVEN ("A call to open() for /bin/sh") {
		auto ret = nihil::open("/bin/sh", nihil::open_read);

		THEN ("The returned file descriptor should be /bin/sh") {
			REQUIRE(ret);

			auto sb1 = nihil::stat(*ret).value();
			auto sb2 = nihil::stat("/bin/sh").value();
			REQUIRE(sb1.st_ino == sb2.st_ino);
			REQUIRE(sb1.st_dev == sb2.st_dev);
		}
	}
}

SCENARIO("nihil::open_in_path returns ENOENT when the file is not found")
{
	GIVEN ("A call to open for a non-existent file") {
		auto ret = nihil::open("/nihil_no_such_file", nihil::open_read);

		THEN ("The return value should be std::errc::no_such_file_or_directory") {
			REQUIRE(!ret);
			REQUIRE(ret.error() == std::errc::no_such_file_or_directory);
		}
	}
}

} // anonymous namespace
