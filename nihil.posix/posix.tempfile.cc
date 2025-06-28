/*
 * This source code is released into the public domain.
 */

module;

#include <algorithm>
#include <coroutine>
#include <expected>
#include <filesystem>
#include <random>
#include <string>

#include <fcntl.h>
#include <unistd.h>

module nihil.posix;

import nihil.flagset;
import :getenv;
import :open;

namespace nihil {

temporary_file::temporary_file(nihil::fd &&fd,
			       std::filesystem::path path) noexcept
	: m_fd(std::move(fd))
	, m_path(std::move(path))
{
}

temporary_file::temporary_file(nihil::fd &&fd) noexcept
	: m_fd(std::move(fd))
{
}

temporary_file::temporary_file(temporary_file &&other) noexcept
	: m_fd(std::move(other.m_fd))
	, m_path(std::move(other.m_path))
{
}

auto temporary_file::operator=(this temporary_file &self,
			       temporary_file &&other)
	noexcept -> temporary_file &
{
	if (&self != &other) {
		if (self.m_fd)
			self.release();

		self.m_fd = std::move(other.m_fd);
		self.m_path = std::move(other.m_path);
	}

	return self;
}

temporary_file::~temporary_file()
{
	if (m_fd)
		release();
}

auto temporary_file::release(this temporary_file &self) -> void
{
	if (!self.m_fd)
		throw std::logic_error(
			"release() called on already-released tempfile");

	if (!self.m_path.empty()) {
		auto ec = std::error_code(); // ignore errors
		remove(self.path(), ec);

		self.m_path.clear();
	}

	std::ignore = self.m_fd.close();
}

auto temporary_file::path(this temporary_file const &self)
	-> std::filesystem::path const &
{
	if (self.m_path.empty())
		throw std::logic_error(
			"path() called on unlinked temporary_file");

	return self.m_path;
}

auto temporary_file::fd(this temporary_file &self) -> nihil::fd &
{
	if (!self.m_fd)
		throw std::logic_error("fd() called on empty temporary_file");

	return self.m_fd;
}

auto tempfile(tempfile_flags_t flags) -> std::expected<temporary_file, error>
{
	auto rng = std::default_random_engine(std::random_device{}());

	auto random_name = [&] -> std::string {
		auto constexpr length = std::size_t{12};
		auto constexpr randchars = std::string_view(
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"0123456789");

		auto dist = std::uniform_int_distribution<>(
				0, randchars.size() - 1);
		auto ret = std::string(length, 0);
		std::ranges::generate_n(ret.begin(), length,
					[&] -> char {
						return randchars[dist(rng)];
					});
		return ret;
	};

	auto dir = std::filesystem::path(getenv("TMPDIR").value_or("/tmp"));

	// Keep trying until we don't get EEXIST.
	for (;;) {
		auto filename = dir / (random_name() + ".tmp");
		auto fd = nihil::open(filename, O_RDWR | O_CREAT | O_EXCL,
				      0600);
		if (!fd) {
			if (fd.error() == std::errc::file_exists)
				continue;
			return std::unexpected(fd.error());
		}

		if (flags & tempfile_unlink) {
			auto ec = std::error_code();
			remove(filename, ec);
			return temporary_file(std::move(*fd));
		} else {
			return temporary_file(std::move(*fd),
					      std::move(filename));
		}
	}
}

} // namespace nihil
