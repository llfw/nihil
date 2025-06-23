/*
 * This source code is released into the public domain.
 */

module;

#include <filesystem>
#include <format>
#include <utility>

import nihil;
import nihil.ucl;

module nihil.config;

namespace nihil::config {

auto write_to(std::filesystem::path const &filename) -> void
try {
	auto uclconfig = ucl::map<ucl::object>();

	// Add all the options to the UCL object.
	for (auto const &option : store::get().all()) {
		if (option.is_default())
			continue;

		uclconfig.insert({option.name(), option.to_ucl()});
	}

	auto ucl_text = std::format("{:c}", uclconfig);
	auto ret = safe_write_file(filename, ucl_text);
	if (!ret)
		throw error(std::format("{}: {}", filename.string(),
					ret.error().message()));
} catch (ucl::error const &exc) {
	throw error(std::format("{}: {}", filename.string(), exc.what()));
}

};
