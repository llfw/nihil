/*
 * This source code is released into the public domain.
 */

module;

#include <filesystem>
#include <optional>
#include <ranges>
#include <string>

#include <fcntl.h>
#include <paths.h>

module nihil;

namespace nihil {

auto find_in_path(std::filesystem::path const &file)
-> std::optional<fd>
{
	using namespace std::literals;

	auto try_open =
	[](std::filesystem::path const &file) -> std::optional<fd>
	{
		auto ret = open_file(file, O_EXEC);
		if (ret)
			return {std::move(*ret)};
		return {};
	};

	// Absolute pathname skips the search.
	if (file.is_absolute())
		return try_open(file);

	auto path = getenv("PATH").value_or(_PATH_DEFPATH);

	for (auto &&dir : path | std::views::split(':')) {
		// An empty $PATH element means cwd.
		auto sdir = dir.empty()
			? std::filesystem::path(".")
			: std::filesystem::path(std::string_view(dir));

		if (auto ret = try_open(sdir / file); ret)
			return ret;
	}

	return {};
}

} // namespace lfjail
