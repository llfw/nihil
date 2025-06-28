/*
 * This source code is released into the public domain.
 */

module;

#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <print>
#include <ranges>
#include <string>
#include <utility>

#include <unistd.h>

module nihil.cli;

import nihil.core;

namespace nihil {

auto dispatch_command(int argc, char **argv) -> int
{
	auto tree = build_command_tree();

	// The caller should have stripped argv[0] already.  find() will
	// strip all the remaining elements except the last, which means
	// argv[0] will be set to something reasonable for the next call
	// to getopt().

	// find() never returns nullptr; at worst it will return the
	// root node.
	auto const *node = tree.find(argc, argv);

	// Get the command_node.
	auto const &command = node->command();

	// Reset getopt(3) for the command, in case main() used it already.
	optreset = 1;
	optind = 1;

	/*
	 * Set the program name to the existing progname plus the full path
	 * to the command being invoked; this makes error messages nicer.
	 */
	auto *old_progname = ::getprogname();

	{
		auto cprogname = std::format("{} {}", ::getprogname(),
					     command->path());
		::setprogname(cprogname.c_str());
	}

	// Invoke the command see what it returns.
	auto ret = command->invoke(argc, argv);

	// Restore the old progname.
	::setprogname(old_progname);

	// If the command produced an exit code, return it.
	if (ret)
		return *ret;

	/*
	 * We have special handling for some errors.
	 */

	// Incomplete command: print the list of valid commands at this node.
	if (ret.error() == errc::incomplete_command) {
		std::print(std::cerr, "{}: usage:\n", ::getprogname());
		node->print_commands();
		return 1;
	}

	// We didn't recognise the error, so just print it and exit.
	std::print(std::cerr, "{}\n", ret.error());
	return 1;
}

} // namespace nihil
