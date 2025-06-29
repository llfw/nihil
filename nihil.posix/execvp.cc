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

#include "nihil.hh"

module nihil.posix;

import nihil.error;
import nihil.monad;

namespace nihil {
#ifdef NIHIL_HAVE_FEXECVE

auto execvp(std::string_view file, argv &&argv) -> std::expected<fexecv, error>
{
	auto execfd = open_in_path(file);
	if (!execfd)
		return std::unexpected(error(
			std::format("executable not found in path: {}", file)));
	return fexecv(std::move(*execfd), std::move(argv));
}

#else	// !NIHIL_HAVE_FEXECVE

auto execvp(std::string_view file, nihil::argv &&argv) -> std::expected<nihil::execv, nihil::error>
{
	auto filename = nihil::find_in_path(file);
	if (!filename)
		return std::unexpected(nihil::error(
			std::format("executable not found in path: {}", file)));
	return execv(std::move(*filename), std::move(argv));
}

#endif	// NIHIL_HAVE_FEXECVE

} // namespace nihil
