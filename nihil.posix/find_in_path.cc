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
#include <unistd.h>

module nihil.posix;

namespace nihil {

auto find_in_path(std::filesystem::path const &file) -> std::optional<std::filesystem::path>
{
	using namespace std::literals;

	auto try_return = [](std::filesystem::path file)
		-> std::optional<std::filesystem::path>
	{
		auto ret = ::access(file.string().c_str(), X_OK);
		if (ret == 0)
			return {std::move(file)};
		return {};
	};

	// Absolute pathname skips the search.
	if (file.is_absolute())
		return try_return(file);

	auto path = getenv("PATH").value_or(_PATH_DEFPATH);

	for (auto &&dir : path | std::views::split(':')) {
		// An empty $PATH element means cwd.
		auto sdir = dir.empty()
			? std::filesystem::path(".")
			: std::filesystem::path(std::string_view(dir));

		if (auto ret = try_return(sdir / file); ret)
			return ret;
	}

	return {};
}

} // namespace nihil
