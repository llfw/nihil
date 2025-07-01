// This source code is released into the public domain.
module;

#include <stdlib.h>   // getprogname, NOLINT
#include <sysexits.h> // EX_USAGE
#include <unistd.h>   // getopt

module nihil.cli;

import nihil.std;
import nihil.core;

namespace nihil {

namespace {
// Print this node's children in a form useful to humans.
auto print_commands(command_tree_node const &node) -> void
{
	for (auto &&child : node.children())
		std::print(std::cerr, "  {}\n", child.command().path());
}
} // namespace

auto dispatch_command(int argc, char **argv) -> int
{
	// Reset getopt(3) for the command, in case main() used it already.
	optreset = 1;
	optind = 1;

	// Node that tree.find() never fails, at worst it will return the root node.
	auto tree = build_command_tree();
	auto [node, rest] = tree.find(std::span(argv, argc));
	auto const &command = node->command();

	// Set the program name to the existing progname plus the full path to the command being
	// invoked; this makes error messages nicer. Save the old progname so we can restore it
	// after invoking the command.
	auto const *old_progname = ::getprogname();

	{
		auto cprogname = std::format("{} {}", ::getprogname(), command.path());
		::setprogname(cprogname.c_str());
	}

	// Invoke the command see what it returns.  If it's an exit code, just return it.
	// Otherwise, handle the error.
	auto nrest = static_cast<int>(std::ranges::distance(rest));
	// Keep the first argument, because getopt() wants it
	if (nrest < argc)
		++nrest;
	auto ret = command.invoke(nrest, argv + (argc - nrest));

	// Restore the old progname.
	::setprogname(old_progname);

	if (ret)
		return *ret;

	// Incomplete command: print the list of valid commands at this node.
	if (ret.error() == errc::incomplete_command) {
		std::println(std::cerr, "{}: usage:", ::getprogname());
		print_commands(*node);
		return EX_USAGE;
	}

	// We didn't recognise the error, so just print it and exit.
	std::println(std::cerr, "{}", ret.error());
	return 1;
}

} // namespace nihil
