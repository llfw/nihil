/*
 * This source code is released into the public domain.
 */

module;

#include <filesystem>
#include <format>
#include <system_error>

module nihil;

namespace nihil {

auto ensure_dir(std::filesystem::path const &dir) -> void
{
	std::error_code err;

	if (std::filesystem::create_directories(dir, err))
		return;

	if (err)
		throw generic_error(std::format("{}: mkdir: {}",
				    dir.string(), err.message()));
}

} // namespace nihil
