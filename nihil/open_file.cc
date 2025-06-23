/*
 * This source code is released into the public domain.
 */

module;

#include <expected>
#include <filesystem>
#include <system_error>

#include <fcntl.h>
#include <unistd.h>

module nihil;

namespace nihil {

auto open_file(std::filesystem::path const &filename, int flags, int mode)
-> std::expected<fd, std::error_code>
{
	auto fdno = ::open(filename.c_str(), flags, mode);
	if (fdno != -1)
		return fd(fdno);

	return std::unexpected(std::make_error_code(std::errc(errno)));
}

} // namespace nihil
