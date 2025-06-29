/*
 * This source code is released into the public domain.
 */

module;

#include <expected>
#include <format>

module nihil.posix;

import nihil.error;
import nihil.monad;

namespace nihil {

auto shell(std::string_view const &command) -> std::expected<base_executor_type, error>
{
	return execl("/bin/sh", "sh", "-c", command);
}

} // namespace nihil
