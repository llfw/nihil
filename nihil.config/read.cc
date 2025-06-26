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

module nihil.config;

import nihil;
import nihil.ucl;

namespace nihil::config {

auto read_from(std::filesystem::path const &filename)
	-> std::expected<void, error>
{
	// TODO: nihil.ucl should have a way to load UCL from a filename.

	std::string config_text;
	auto err = read_file(filename, std::back_inserter(config_text));
	if (!err) {
		// Ignore ENOENT, it simply means we haven't created the
		// config file yet, so default values will be used.
		if (err.error().root_cause() == std::errc::no_such_file_or_directory)
			co_return {};
		auto errstr = std::format("cannot read {}", filename.string());
		co_return std::unexpected(error(errstr, err.error()));
	}

	// Parse the UCL.
	auto uclconfig = co_await ucl::parse(config_text);

	for (auto &&[key, value] : uclconfig) {
		auto opt = co_await store::get().fetch(key);
		co_await opt->ucl(value);
	}

	co_return {};
}

} // namespace nihil::config
