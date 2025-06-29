/*
 * This source code is released into the public domain.
 */

module;

#include <cstdint>
#include <expected>
#include <string>
#include <system_error>
#include <vector>

#include <unistd.h>

#include "nihil.hh"

module nihil.posix;

import nihil.error;

namespace nihil {

auto getenv(std::string_view varname) -> std::expected<std::string, error>
{
	auto cvarname = std::string(varname);

#ifdef NIHIL_HAVE_GETENV_R
	// Start with a buffer of this size, and double it every iteration.
	constexpr auto bufinc = std::size_t{1024};

	auto buf = std::vector<char>(bufinc);
	for (;;) {
		auto const ret = ::getenv_r(cvarname.c_str(),
					    buf.data(), buf.size());

		if (ret == 0)
			return {std::string(buf.data())};

		if (ret == -1 && errno == ERANGE) {
			buf.resize(buf.size() * 2);
			continue;
		}

		return std::unexpected(error(std::errc(errno)));
	}
#else // NIHIL_HAVE_GETENV_R
	auto *v = ::getenv(cvarname.c_str());
	if (v == nullptr)
		return std::unexpected(error(std::errc(errno)));
	return {std::string(v)};
#endif // NIHIL_HAVE_GETENV_R
}

} // namespace nihil
