/*
 * This source code is released into the public domain.
 */

module;

#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <utility>

#include <unistd.h>

module nihil;

/*
 * command_map represents a hierarchical list of commands.  At each level,
 * a command is mapped to a handler, which can either be a function, in
 * which case we execute the function, or another command_map, in which
 * case we invoke the new map
 */

namespace nihil {

/*
 * The string tree we store our commands in.  This is sort of like a very
 * basic hierarchical std::map.  Keys are provided as a range of values,
 * typically from argv.
 */

struct command_tree_node final {
	command_tree_node()
		: _this_word("")
	{
	}

	command_tree_node(std::string_view this_word)
		: _this_word(this_word)
	{
	}

	command_tree_node(std::string_view this_word,
			  command value)
		: _this_word(this_word)
		, _value(std::move(value))
	{
	}

	/*
	 * Return a child node, or NULL if the child doesn't exist.
	 */
	auto get_child(this command_tree_node const &self,
		       std::string_view child)
		-> command_tree_node const *
	{
		if (auto it = self.children.find(std::string(child));
		    it != self.children.end())
			return &it->second;

		return nullptr;
	}

	auto get_child(this command_tree_node &self,
		       std::string_view child)
		-> command_tree_node *
	{
		if (auto it = self.children.find(std::string(child));
		    it != self.children.end())
			return &it->second;

		return nullptr;
	}

	/*
	 * Return a child node if it exists, or insert a new empty node.
	 */
	auto get_or_create_child(this command_tree_node &self,
				 std::string_view child)
		-> command_tree_node *
	{
		if (auto ptr = self.get_child(child); ptr != nullptr)
			return ptr;

		auto [it, ok] = self.children.emplace(
					child, command_tree_node(child));
		return &it->second;
	}

	/*
	 * Return this node's value.
	 */
	auto value(this command_tree_node const &self)
		-> std::optional<command> const &
	{
		return self._value;
	}

	/*
	 * Set this node's value.
	 */
	auto value(this command_tree_node &self, command value) -> void
	{
		self._value = std::move(value);
	}

	/*
	 * Print this node's children in a form useful to humans.
	 */
	auto print_commands(this command_tree_node const &self,
			    std::string_view prefix) -> void
	{
		for (auto &&[name, node] : self.children) {
			std::print("  {} {}\n", prefix, name);
		}
	}

private:
	std::string _this_word;
	std::optional<command> _value;
	std::unordered_map<std::string, command_tree_node> children;
};

struct command_tree {
	/*
	 * Add a node to the tree.  Returns false if the node already exists.
	 */
	auto insert(this command_tree &self,
		    std::ranges::range auto &&path,
		    command value)
		-> bool
	{
		auto *this_node = &self._root_node;

		// Find the node for this key.
		for (auto &&next : path) {
			auto this_word = std::string_view(next);
			this_node = this_node->get_or_create_child(this_word);
		}

		if (this_node->value()) {
			// The value already exists.
			return false;
		}

		// Set the new value.
		this_node->value(std::move(value));
		return true;
	}

	/*
	 * Find a node in the tree.  Unlike insert(), iteration stops when
	 * we find any node with a value.
	 */
	auto find(this command_tree const &self, int &argc, char **&argv)
		-> std::optional<command>
	{
		auto *this_node = &self._root_node;

		// Assume the caller already stripped the program name from
		// argv.  This is usually the case since they would have
		// called getopt().

		// Store the command bits we got so far, so we can print them
		// in the usage error if needed.
		auto path = std::string();

		while (argv[0] != nullptr) {
			auto next = std::string_view(argv[0]);

			auto *next_node = this_node->get_child(next);

			if (next_node == nullptr) {
				// The node doesn't exist, so this command is
				// not valid.  Print a list of valid commands.
				std::print(std::cerr,
					   "{}: unknown command: {} {}\n",
					   ::getprogname(), path, next);
				std::print(std::cerr,
					   "{}: expected one of:\n",
					   ::getprogname());

				this_node->print_commands(path);
				return {};
			}

			this_node = next_node;

			if (this_node->value())
				// This node has a value, so return it.
				return {this_node->value()};

			if (!path.empty())
				path += ' ';
			path += next;
			--argc;
			++argv;
		}

		// We didn't find a value, so the key was incomplete.
		std::print(std::cerr, "{}: {} command; expected:\n",
			   ::getprogname(),
			   path.empty() ? "missing" : "incomplete");
		this_node->print_commands(path);

		return {};
	}

private:
	command_tree_node _root_node;
};

/*
 * The global command map.
 */
auto get_commands() -> command_tree & {
	static auto commands = command_tree();
	return commands;
}

auto register_command(std::string_view path, command *cmd) noexcept -> void
try {
	auto &commands = get_commands();
	if (commands.insert(path | std::views::split(' '), *cmd) == false) {
		std::printf("command registration failed\n");
		std::abort();
	}
} catch (...) {
	std::printf("command registration failed\n");
	std::abort();
}

auto dispatch_command(int argc, char **argv) -> int
{
	auto &commands = get_commands();

	// The caller should have stripped argv[0] already.  find() will
	// strip all the remaining elements except the last, which means
	// argv[0] will be set to something reasonable for the next call
	// to getopt().

	auto node = commands.find(argc, argv);

	if (!node)
		// find() already printed the error message
		return 1;

	// Reset getopt here for the command to use.
	optreset = 1;
	optind = 1;

	// Calling setprogname() makes error messages more relevant.
	auto cprogname = std::format("{} {}", ::getprogname(),
				     node->path());
	::setprogname(cprogname.c_str());

	return node->invoke(argc, argv);
}

void print_usage(std::string_view)
{
//	get_root_node().print_usage(std::string(prefix));
}

} // namespace nihil
