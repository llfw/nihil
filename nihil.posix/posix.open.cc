/*
 * This source code is released into the public domain.
 */

module;

#include <expected>
#include <filesystem>
#include <system_error>

#include <fcntl.h>
#include <unistd.h>

module nihil.posix;

import nihil.error;
import :fd;

namespace nihil {

auto open(std::filesystem::path const &filename, int flags, int mode)
	-> std::expected<fd, error>
{
	auto fdno = ::open(filename.c_str(), flags, mode);
	if (fdno != -1)
		return fd(fdno);

	return std::unexpected(error(std::errc(errno)));
}

} // namespace nihil
