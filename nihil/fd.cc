/*
 * This source code is released into the public domain.
 */

module;

#include <fcntl.h>
#include <unistd.h>

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

fd::fd(int fd_) noexcept
	: _fd(fd_)
{
}

fd::~fd()
{
	if (*this)
		this->close();
}

fd::fd(fd &&other) noexcept
	: _fd(std::exchange(other._fd, _invalid_fd))
{
}

auto fd::operator=(fd &&other) noexcept -> fd &
{
	if (this != &other)
		_fd = std::exchange(other._fd, _invalid_fd);
	return *this;
}

fd::operator bool(this fd const &self) noexcept
{
	return self._fd != _invalid_fd;
}

auto fd::close(this fd &self) -> std::expected<void, std::error_code>
{
	auto const ret = ::close(self.get());
	self._fd = _invalid_fd;

	if (ret == 0)
		return {};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto fd::get(this fd const &self) -> int
{
	if (self)
		return self._fd;
	throw fd_logic_error("Attempt to call get() on invalid fd");
}

auto fd::release(this fd &&self) -> int
{
	if (self)
		return std::exchange(self._fd, self._invalid_fd);
	throw fd_logic_error("Attempt to release an invalid fd");
}

auto dup(fd const &self) -> std::expected<fd, std::error_code>
{
	auto thisfd = self.get();

	auto const newfd = ::dup(thisfd);
	if (newfd != -1)
		return {newfd};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto dup(fd const &self, int newfd) -> std::expected<fd, std::error_code>
{
	auto thisfd = self.get();

	auto const ret = ::dup2(thisfd, newfd);
	if (ret != -1)
		return {newfd};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto raw_dup(fd const &self) -> std::expected<int, std::error_code>
{
	auto thisfd = self.get();

	auto const newfd = ::dup(thisfd);
	if (newfd != -1)
		return {newfd};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto raw_dup(fd const &self, int newfd) -> std::expected<int, std::error_code>
{
	auto thisfd = self.get();

	auto const ret = ::dup2(thisfd, newfd);
	if (ret != -1)
		return {newfd};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto getflags(fd const &self) -> std::expected<int, std::error_code>
{
	auto const flags = ::fcntl(self.get(), F_GETFL);
	if (flags != -1)
		return {flags};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto replaceflags(fd &self, int newflags)
	-> std::expected<void, std::error_code>
{
	auto const ret = ::fcntl(self.get(), F_SETFL, newflags);
	if (ret == 0)
		return {};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto setflags(fd &self, int newflags)
	-> std::expected<int, std::error_code>
{
	auto flags = getflags(self);
	if (!flags)
		return flags;

	*flags |= newflags;
	auto const ret = replaceflags(self, *flags);
	if (!ret)
		return std::unexpected(ret.error());

	return {*flags};
}

auto clearflags(fd &self, int clrflags)
	-> std::expected<int, std::error_code>
{
	auto flags = getflags(self);
	if (!flags)
		return flags;

	*flags &= ~clrflags;
	auto const ret = replaceflags(self, *flags);
	if (!ret)
		return std::unexpected(ret.error());

	return {*flags};
}

auto getfdflags(fd const &self) -> std::expected<int, std::error_code>
{
	auto const flags = ::fcntl(self.get(), F_GETFD);
	if (flags != -1)
		return {flags};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto replacefdflags(fd &self, int newflags)
	-> std::expected<void, std::error_code>
{
	auto const ret = ::fcntl(self.get(), F_SETFD, newflags);
	if (ret != -1)
		return {};

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto setfdflags(fd &self, int newflags)
	-> std::expected<int, std::error_code>
{
	auto flags = getfdflags(self);
	if (!flags)
		return flags;

	*flags |= newflags;
	auto const ret = replacefdflags(self, *flags);
	if (!ret)
		return std::unexpected(ret.error());

	return {*flags};
}

auto clearfdflags(fd &self, int clrflags)
	-> std::expected<int, std::error_code>
{
	auto flags = getfdflags(self);
	if (!flags)
		return flags;

	*flags &= ~clrflags;
	auto ret = replacefdflags(self, *flags);
	if (!ret)
		return std::unexpected(ret.error());

	return {*flags};
}

auto pipe() -> std::expected<std::pair<fd, fd>, std::error_code> {
	auto fds = std::array<int, 2>{};

	if (auto const ret = ::pipe(fds.data()); ret != 0)
		return std::unexpected(std::make_error_code(std::errc(errno)));

	return {{fd(fds[0]), fd(fds[1])}};
}

auto fd::write(this fd &self, std::span<std::byte const> buffer)
	-> std::expected<std::size_t, std::error_code>
{
	auto const ret = ::write(self.get(), buffer.data(), buffer.size());
	if (ret >= 0)
		return ret;

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

auto fd::read(this fd &self, std::span<std::byte> buffer)
	-> std::expected<std::size_t, std::error_code>
{
	auto const ret = ::read(self.get(), buffer.data(), buffer.size());
	if (ret >= 0)
		return ret;

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

} // namespace nihil
