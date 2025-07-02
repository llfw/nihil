// This source code is released into the public domain.

#include <fcntl.h>
#include <unistd.h>

#include <catch2/catch_test_macros.hpp>

import nihil.std;
import nihil.posix;
import nihil.util;

using namespace std::literals;

namespace {

// Test if an fd is open.
auto fd_is_open(int fd) -> bool {
	auto const ret = ::fcntl(fd, F_GETFL);
	return ret == 0;
}

TEST_CASE("fd: construct empty", "[fd]") {
	auto const fd = nihil::fd();

	REQUIRE(!fd);
	REQUIRE_THROWS_AS(fd.get(), std::logic_error);
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
	REQUIRE(!fd1); //NOLINT
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

	REQUIRE(!fd1); //NOLINT
	REQUIRE(fd2);
	REQUIRE(fd2.get() == file);
	REQUIRE(fd_is_open(file));
}

TEST_CASE("fd: release", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd = nihil::fd(file);
	auto fdesc = std::move(fd).release();
	REQUIRE(!fd); //NOLINT
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
	auto constexpr test_fd = 666;
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	REQUIRE(!fd_is_open(test_fd));

	auto fd = nihil::fd(file);
	auto fd2 = dup(fd, test_fd);

	REQUIRE(fd);
	REQUIRE(fd2);
	REQUIRE(fd2->get() == test_fd);
}

TEST_CASE("fd: flags", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd = nihil::fd(file);

	{
		auto const ret = replaceflags(fd, nihil::fd_none);
		REQUIRE(ret);
		REQUIRE(getflags(fd) == nihil::fd_none);
	}

	{
		auto const ret = setflags(fd, nihil::fd_nonblock);
		REQUIRE(ret == nihil::fd_nonblock);
		REQUIRE(getflags(fd) == nihil::fd_nonblock);
	}

	{
		auto const ret = setflags(fd, nihil::fd_sync);
		REQUIRE(ret == (nihil::fd_nonblock | nihil::fd_sync));
		REQUIRE(getflags(fd) == (nihil::fd_nonblock | nihil::fd_sync));
	}

	{
		auto const ret = clearflags(fd, nihil::fd_nonblock);
		REQUIRE(ret == nihil::fd_sync);
		REQUIRE(getflags(fd) == nihil::fd_sync);
	}
}

TEST_CASE("fd: fdflags", "[fd]") {
	auto file = ::open("/dev/null", O_RDONLY);
	REQUIRE(file > 0);

	auto fd = nihil::fd(file);

	{
		auto const ret = replacefdflags(fd, nihil::fd_fd_none);
		REQUIRE(ret);
		REQUIRE(getfdflags(fd) == nihil::fd_fd_none);
	}

	{
		auto const ret = setfdflags(fd, nihil::fd_fd_cloexec);
		REQUIRE(ret == nihil::fd_fd_cloexec);
		REQUIRE(getfdflags(fd) == nihil::fd_fd_cloexec);
	}

	{
		auto const ret = clearfdflags(fd, nihil::fd_fd_cloexec);
		REQUIRE(ret == nihil::fd_fd_none);
		REQUIRE(getfdflags(fd) == nihil::fd_fd_none);
	}
}

TEST_CASE("fd: pipe, read, write", "[fd]") {
	auto fds = nihil::pipe();
	REQUIRE(fds);

	/*
	 * Note: traditionally, the first fd is the reading side, and the second fd
	 * is the writing side.  Some platforms (e.g., macOS) still behave this way.
	 */

	auto [fd1, fd2] = std::move(*fds);

	auto constexpr test_string = "test string"sv;

	auto ret = write(fd2, test_string);
	REQUIRE(ret);
	REQUIRE(*ret == test_string.size());

	auto readbuf = std::array<char, test_string.size() * 2>{};
	auto read_buf = read(fd1, readbuf);
	REQUIRE(read_buf);
	REQUIRE(std::string_view(*read_buf) == test_string);
}

} // anonymous namespace
