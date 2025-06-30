/*
 * This source code is released into the public domain.
 */

#include <filesystem>

#include <catch2/catch_test_macros.hpp>

import nihil.posix;

TEST_CASE("posix.tempfile: create", "[nihil][nihil.posix]")
{
	auto file = nihil::tempfile();
	REQUIRE(file);
	REQUIRE(file->fd());

	auto path = file->path();
	REQUIRE(exists(path) == true);
}

TEST_CASE("posix.tempfile: create and release", "[nihil][nihil.posix]")
{
	auto file = nihil::tempfile();
	REQUIRE(file);
	REQUIRE(file->fd());

	auto path = file->path();
	REQUIRE(exists(path) == true);

	file->release();
	REQUIRE(exists(path) == false);

	REQUIRE_THROWS_AS(file->fd(), std::logic_error);
	REQUIRE_THROWS_AS(file->path(), std::logic_error);
}

TEST_CASE("posix.tempfile: create and double release", "[nihil][nihil.posix]")
{
	auto file = nihil::tempfile();
	REQUIRE(file->fd());

	auto path = file->path();
	REQUIRE(exists(path) == true);

	file->release();
	REQUIRE(exists(path) == false);

	REQUIRE_THROWS_AS(file->fd(), std::logic_error);
	REQUIRE_THROWS_AS(file->release(), std::logic_error);
	REQUIRE_THROWS_AS(file->path(), std::logic_error);
}

TEST_CASE("posix.tempfile: create unlinked", "[nihil][nihil.posix]")
{
	auto file = nihil::tempfile(nihil::tempfile_unlink);
	REQUIRE(file);
	REQUIRE(file->fd());

	REQUIRE_THROWS_AS(file->path(), std::logic_error);
}

TEST_CASE("posix.tempfile: create unlinked and release",
	  "[nihil][nihil.posix]")
{
	auto file = nihil::tempfile(nihil::tempfile_unlink);
	REQUIRE(file);
	REQUIRE(file->fd());

	REQUIRE_THROWS_AS(file->path(), std::logic_error);

	file->release();

	REQUIRE_THROWS_AS(file->fd(), std::logic_error);
	REQUIRE_THROWS_AS(file->path(), std::logic_error);
}

TEST_CASE("posix.tempfile: create unlinked and double release",
	  "[nihil][nihil.posix]")
{
	auto file = nihil::tempfile(nihil::tempfile_unlink);
	REQUIRE(file->fd());

	REQUIRE_THROWS_AS(file->path(), std::logic_error);

	file->release();

	REQUIRE_THROWS_AS(file->fd(), std::logic_error);
	REQUIRE_THROWS_AS(file->release(), std::logic_error);
	REQUIRE_THROWS_AS(file->path(), std::logic_error);
}
