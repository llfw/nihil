/*
 * This source code is released into the public domain.
 */

module;

#include <expected>
#include <iostream>
#include <print>
#include <string>

module nihil.cli;

import nihil.core;
import nihil.error;

namespace nihil {

command_node::command_node(std::string_view path) noexcept
	: m_path(path)
{
}

command_node::~command_node()
{
}

auto command_node::path(this command_node const &self) noexcept
	-> std::string_view
{
	return self.m_path;
}

auto command_node::invoke(int, char **) const
	-> std::expected<int, error>
{
	// If invoke() wasn't overridden, then this is an empty node,
	// so the command was incomplete.
	return std::unexpected(error(errc::incomplete_command));
}

} // namespace nihil
