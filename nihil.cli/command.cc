/*
 * This source code is released into the public domain.
 */

module;

#include <expected>
#include <functional>
#include <iostream>
#include <print>
#include <string>

// For EX_USAGE.  While <sysexits.h> is deprecated, there's no other standard
// exit code for 'usage error'; some programs use 2 (common on Linux), but
// 2 is also used for many other exit codes.
#include <sysexits.h>

module nihil.cli;

import nihil.error;
import :registry;

namespace nihil {

//NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
command::command(std::string_view path, std::string_view usage,
		 command_function_t handler)
	: command_node(path)
	, m_usage(usage)
	, m_handler(std::move(handler))
{
	register_command(this);
}

auto command::usage(this command const &self) noexcept -> std::string_view
{
	return self.m_usage;
}

auto command::invoke(int argc, char **argv) const
	-> std::expected<int, error>
{
	try {
		return std::invoke(m_handler, argc, argv);
	} catch (usage_error const &err) {
		std::print(std::cerr, "{}\n", err.what());
		std::print(std::cerr, "usage: {} {}", path(), usage());
		return EX_USAGE;
	}
}

} // namespace nihil
