/*
 * This source code is released into the public domain.
 */

module;

#include <coroutine>
#include <expected>
#include <filesystem>
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

execv::execv(std::filesystem::path path, argv &&args) noexcept
	: m_path(std::move(path))
	, m_args(std::move(args))
{
}

auto execv::exec(this execv &self) -> std::expected<void, error>
{
	::execve(self.m_path.string().c_str(), self.m_args.data(), environ);
	return std::unexpected(error("execve failed", error(std::errc(errno))));
}

execv::execv(execv &&) noexcept = default;
auto execv::operator=(this execv &, execv &&) noexcept -> execv & = default;

} // namespace nihil
