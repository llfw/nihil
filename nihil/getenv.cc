
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

module nihil;

namespace nihil {

 auto getenv(std::string_view varname)
-> std::expected<std::string, std::error_code>
{
	// Start with a buffer of this size, and double it every iteration.
	constexpr auto bufinc = std::size_t{1024};

	auto cvarname = std::string(varname);
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

		return std::unexpected(std::make_error_code(std::errc(errno)));
	}
}

} // namespace nihil
