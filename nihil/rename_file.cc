/*
 * This source code is released into the public domain.
 */

module;

#include <expected>
#include <filesystem>

module nihil;

namespace nihil {

/*
 * Rename a file.
 */
auto rename_file(std::filesystem::path const &oldp,
		 std::filesystem::path const &newp)
	-> std::expected<void, error>
{
	auto err = std::error_code();

	std::filesystem::rename(oldp, newp, err);

	if (err)
		return std::unexpected(error(err));

	return {};
}


} // namespace nihil
