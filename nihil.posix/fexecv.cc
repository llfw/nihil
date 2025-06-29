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

#include "nihil.h"

#ifdef NIHIL_HAVE_FEXECVE

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

} // namespace nihil

#endif	// NIHIL_HAVE_FEXECVE
