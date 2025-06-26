/*
 * This source code is released into the public domain.
 */

module;

#include <coroutine>
#include <expected>
#include <filesystem>
#include <format>
#include <iterator>
#include <string>

import nihil;
import nihil.ucl;

module nihil.config;

namespace nihil::config {

auto read_from(std::filesystem::path const &filename)
	-> std::expected<void, nihil::error>
{
	// TODO: nihil.ucl should have a way to load UCL from a filename.

	std::string config_text;
	auto err = read_file(filename, std::back_inserter(config_text));
	if (!err) {
		// Ignore ENOENT, it simply means we haven't created the
		// config file yet, so default values will be used.
		if (err.error().root_cause() == std::errc::no_such_file_or_directory)
			return {};
		auto errstr = std::format("cannot read {}", filename.string());
		return std::unexpected(nihil::error(errstr, err.error()));
	}

	// Parse the UCL.
	try {
		auto uclconfig = ucl::parse(config_text);

		for (auto &&[key, value] : uclconfig) {
			auto &opt = store::get().fetch(key);
			opt.from_ucl(value);
		}
	} catch (unknown_option const &) {
		// This is probably an old option which has been removed;
		// ignore it, and we'll remove the bad option next time
		// we write the config.
	} catch (error const &err) {
		// Include the filename in any other config errors.
		throw error(std::format("{}: {}", filename.string(),
					err.what()));
	} catch (ucl::error const &err) {
		throw error(std::format("{}: {}", filename.string(),
					err.what()));
	}

	return {};
}

} // namespace nihil::config
