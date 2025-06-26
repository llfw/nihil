/*
 * This source code is released into the public domain.
 */

module;

#include <fcntl.h>
#include <unistd.h>

#include <coroutine>
#include <expected>
#include <format>
#include <stdexcept>
#include <system_error>

module nihil;

namespace nihil {

fd_logic_error::fd_logic_error(std::string what)
	: std::logic_error(std::move(what))
{
}

fd::fd() noexcept = default;

fd::fd(int fileno) noexcept
	: m_fileno(fileno)
{
}

fd::~fd()
{
	if (*this)
		std::ignore = this->close();
}

fd::fd(fd &&other) noexcept
	: m_fileno(std::exchange(other.m_fileno, invalid_fileno))
{
}

auto fd::operator=(this fd &self, fd &&other) noexcept -> fd &
{
	if (&self != &other)
		self.m_fileno = std::exchange(other.m_fileno, invalid_fileno);
	return self;
}

fd::operator bool(this fd const &self) noexcept
{
	return self.m_fileno != invalid_fileno;
}

auto fd::close(this fd &self) -> std::expected<void, error>
{
	auto const ret = ::close(self.get());
	self.m_fileno = invalid_fileno;

	if (ret == 0)
		return {};

	return std::unexpected(error(std::errc(errno)));
}

auto fd::get(this fd const &self) -> int
{
	if (self)
		return self.m_fileno;
	throw fd_logic_error("Attempt to call get() on invalid fd");
}

auto fd::release(this fd &&self) -> int
{
	if (self)
		return std::exchange(self.m_fileno, invalid_fileno);
	throw fd_logic_error("Attempt to release an invalid fd");
}

auto dup(fd const &self) -> std::expected<fd, error>
{
	auto const newfd = ::dup(self.get());
	if (newfd != -1)
		return newfd;

	return std::unexpected(error(std::errc(errno)));
}

auto dup(fd const &self, int newfd) -> std::expected<fd, error>
{
	auto const ret = ::dup2(self.get(), newfd);
	if (ret != -1)
		return newfd;

	return std::unexpected(error(std::errc(errno)));
}

auto raw_dup(fd const &self) -> std::expected<int, error>
{
	auto const newfd = ::dup(self.get());
	if (newfd != -1)
		return newfd;

	return std::unexpected(error(std::errc(errno)));
}

auto raw_dup(fd const &self, int newfd) -> std::expected<int, error>
{
	auto const ret = ::dup2(self.get(), newfd);
	if (ret != -1)
		return newfd;

	return std::unexpected(error(std::errc(errno)));
}

auto getflags(fd const &self) -> std::expected<int, error>
{
	auto const flags = ::fcntl(self.get(), F_GETFL);
	if (flags != -1)
		return flags;

	return std::unexpected(error(std::errc(errno)));
}

auto replaceflags(fd &self, int newflags) -> std::expected<void, error>
{
	auto const ret = ::fcntl(self.get(), F_SETFL, newflags);
	if (ret == 0)
		return {};

	return std::unexpected(error(std::errc(errno)));
}

auto setflags(fd &self, int newflags) -> std::expected<int, error>
{
	auto flags = co_await getflags(self);

	flags |= newflags;
	co_await replaceflags(self, flags);

	co_return flags;
}

auto clearflags(fd &self, int clrflags) -> std::expected<int, error>
{
	auto flags = co_await getflags(self);

	flags &= ~clrflags;
	co_await replaceflags(self, flags);

	co_return flags;
}

auto getfdflags(fd const &self) -> std::expected<int, error>
{
	auto const flags = ::fcntl(self.get(), F_GETFD);
	if (flags != -1)
		return flags;

	return std::unexpected(error(std::errc(errno)));
}

auto replacefdflags(fd &self, int newflags) -> std::expected<void, error>
{
	auto const ret = ::fcntl(self.get(), F_SETFD, newflags);
	if (ret != -1)
		return {};

	return std::unexpected(error(std::errc(errno)));
}

auto setfdflags(fd &self, int newflags) -> std::expected<int, error>
{
	auto flags = co_await getfdflags(self);

	flags |= newflags;
	co_await replacefdflags(self, flags);

	co_return flags;
}

auto clearfdflags(fd &self, int clrflags) -> std::expected<int, error>
{
	auto flags = co_await getfdflags(self);

	flags &= ~clrflags;
	co_await replacefdflags(self, flags);

	co_return flags;
}

auto pipe() -> std::expected<std::pair<fd, fd>, error>
{
	auto fds = std::array<int, 2>{};

	if (auto const ret = ::pipe(fds.data()); ret != 0)
		return std::unexpected(error(std::errc(errno)));

	return {{fd(fds[0]), fd(fds[1])}};
}

auto fd::write(this fd &self, std::span<std::byte const> buffer)
	-> std::expected<std::size_t, error>
{
	auto const ret = ::write(self.get(), buffer.data(), buffer.size());
	if (ret >= 0)
		return ret;

	return std::unexpected(error(std::errc(errno)));
}

auto fd::read(this fd &self, std::span<std::byte> buffer)
	-> std::expected<std::span<std::byte>, error>
{
	auto const ret = ::read(self.get(), buffer.data(), buffer.size());
	if (ret >= 0)
		return buffer.subspan(0, ret);

	return std::unexpected(error(std::errc(errno)));
}

} // namespace nihil
