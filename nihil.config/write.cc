/*
 * This source code is released into the public domain.
 */

module;

#include <coroutine>
#include <expected>
#include <filesystem>
#include <format>
#include <utility>

module nihil.config;

import nihil;
import nihil.ucl;

namespace nihil::config {

auto write_to(std::filesystem::path const &filename)
	-> std::expected<void, error>
{
	auto uclconfig = ucl::map<ucl::object>();

	// Add all the options to the UCL object.
	for (auto const &option : store::get().all()) {
		if (option->is_default())
			continue;

		auto uobj = co_await option->ucl();
		uclconfig.insert({option->name(), uobj});
	}

	auto ucl_text = std::format("{:c}", uclconfig);
	co_await safe_write_file(filename, ucl_text);
	co_return {};
}

};
