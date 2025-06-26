/*
 * This source code is released into the public domain.
 */

module;

#include <expected>
#include <filesystem>
#include <format>
#include <system_error>

module nihil;

namespace nihil {

auto ensure_dir(std::filesystem::path const &dir) -> std::expected<void, error>
{
	auto err = std::error_code();

	std::filesystem::create_directories(dir, err);
	
	if (err)
		return std::unexpected(error(err));

	return {};
}

} // namespace nihil
