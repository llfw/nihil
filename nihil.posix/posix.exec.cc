/*
 * This source code is released into the public domain.
 */

module;

#include <coroutine>
#include <expected>
#include <format>
#include <string>
#include <utility>

#include <err.h>
#include <fcntl.h>
#include <unistd.h>

extern char **environ;

module nihil.posix;

import nihil.error;
import nihil.monad;

namespace nihil {

fexecv::fexecv(fd &&execfd, argv &&args) noexcept
	: m_execfd(std::move(execfd))
	, m_args(std::move(args))
{
}

auto fexecv::exec(this fexecv &self)
	-> std::expected<void, error>
{
	::fexecve(self.m_execfd.get(), self.m_args.data(), environ);
	return std::unexpected(error("fexecve failed",
				     error(std::errc(errno))));
}
	
fexecv::fexecv(fexecv &&) noexcept = default;
auto fexecv::operator=(this fexecv &, fexecv &&) noexcept -> fexecv& = default;
	
auto execv(std::string_view path, argv &&argv)
	-> std::expected<fexecv, error>
{
	auto file = co_await open(path, O_EXEC)
		.transform_error([&] (error cause) {
			return error(std::format("could not open {}", path),
				     std::move(cause));
		});

	co_return fexecv(std::move(file), std::move(argv));
}

auto execvp(std::string_view file, argv &&argv)
	-> std::expected<fexecv, error>
{
	auto execfd = find_in_path(file);
	if (!execfd)
		return std::unexpected(error(
			std::format("executable not found in path: {}", file)));
	return fexecv(std::move(*execfd), std::move(argv));
}

auto shell(std::string_view const &command)
	-> std::expected<fexecv, error>
{
	return execl("/bin/sh", "sh", "-c", command);
}

} // namespace nihil
