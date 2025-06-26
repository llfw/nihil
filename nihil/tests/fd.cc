/*
 * This source code is released into the public domain.
 */

#include <span>

#include <stdio.h>
#include <fcntl.h>

#include <catch2/catch_test_macros.hpp>

import nihil;

using namespace std::literals;

namespace {

// Test if an fd is open.
auto fd_is_open(int fd) -> bool {
	auto const ret = ::fcntl(fd, F_GETFL);
	return ret == 0;
}

} // anonymous namespace

TEST_CASE("fd: construct empty", "[fd]") {
	nihil::fd fd;

	REQUIRE(!fd);
	REQUIRE_THROWS_AS(fd.get(), nihil::fd_logic_error);
}

TEST_CASE("fd: construct from fd", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	{
		auto fd = nihil::fd(file);
		REQUIRE(fd_is_open(fd.get()));
	}

	REQUIRE(!fd_is_open(file));
}

TEST_CASE("fd: close", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd = nihil::fd(file);
	REQUIRE(fd);

	auto const ret = fd.close();
	REQUIRE(ret);
	REQUIRE(!fd_is_open(file));
}

TEST_CASE("fd: move construct", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd1 = nihil::fd(file);
	REQUIRE(fd_is_open(fd1.get()));

	auto fd2(std::move(fd1));
	REQUIRE(!fd1);
	REQUIRE(fd2);
	REQUIRE(fd2.get() == file);
	REQUIRE(fd_is_open(file));
}

TEST_CASE("fd: move assign", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd1 = nihil::fd(file);
	REQUIRE(fd_is_open(fd1.get()));

	auto fd2 = nihil::fd();
	REQUIRE(!fd2);

	fd2 = std::move(fd1);

	REQUIRE(!fd1);
	REQUIRE(fd2);
	REQUIRE(fd2.get() == file);
	REQUIRE(fd_is_open(file));
}

TEST_CASE("fd: release", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd = nihil::fd(file);
	auto fdesc = std::move(fd).release();
	REQUIRE(!fd);
	REQUIRE(fdesc == file);
}

TEST_CASE("fd: dup", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd = nihil::fd(file);
	REQUIRE(fd);

	auto fd2 = dup(fd);
	REQUIRE(fd2);
	REQUIRE(fd.get() != fd2->get());
}

TEST_CASE("fd: dup2", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	REQUIRE(!fd_is_open(666));

	auto fd = nihil::fd(file);
	auto fd2 = dup(fd, 666);

	REQUIRE(fd);
	REQUIRE(fd2);
	REQUIRE(fd2->get() == 666);
}

TEST_CASE("fd: flags", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd = nihil::fd(file);

	{
		auto const ret = replaceflags(fd, 0);
		REQUIRE(ret);
		REQUIRE(getflags(fd) == 0);
	}

	{
		auto const ret = setflags(fd, O_NONBLOCK);
		REQUIRE(ret == O_NONBLOCK);
		REQUIRE(getflags(fd) == O_NONBLOCK);
	}

	{
		auto const ret = setflags(fd, O_SYNC);
		REQUIRE(ret == (O_NONBLOCK|O_SYNC));
		REQUIRE(getflags(fd) == (O_NONBLOCK|O_SYNC));
	}

	{
		auto const ret = clearflags(fd, O_NONBLOCK);
		REQUIRE(ret == O_SYNC);
		REQUIRE(getflags(fd) == O_SYNC);
	}
}

TEST_CASE("fd: fdflags", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd = nihil::fd(file);

	{
		auto const ret = replacefdflags(fd, 0);
		REQUIRE(ret);
		REQUIRE(getfdflags(fd) == 0);
	}

	{
		auto const ret = setfdflags(fd, FD_CLOEXEC);
		REQUIRE(ret == FD_CLOEXEC);
		REQUIRE(getfdflags(fd) == FD_CLOEXEC);
	}

	{
		auto const ret = clearfdflags(fd, FD_CLOEXEC);
		REQUIRE(ret == 0);
		REQUIRE(getfdflags(fd) == 0);
	}
}

TEST_CASE("fd: pipe, read, write", "[fd]") {
	auto fds = nihil::pipe();
	REQUIRE(fds);

	auto [fd1, fd2] = std::move(*fds);

	auto constexpr test_string = "test string"sv;

	auto ret = write(fd1, test_string);
	REQUIRE(ret);
	REQUIRE(*ret == test_string.size());

	auto readbuf = std::array<char, test_string.size() * 2>{};
	auto read_buf = read(fd2, readbuf);
	REQUIRE(read_buf);
	REQUIRE(std::string_view(*read_buf) == test_string);
}
