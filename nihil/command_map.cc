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

template<typename T>
struct string_tree_node final {
	string_tree_node()
		: _this_word("")
	{
	}

	string_tree_node(std::string_view this_word)
		: _this_word(this_word)
	{
	}

	string_tree_node(std::string_view this_word, T value)
		: _this_word(this_word)
		, _value(std::move(value))
	{
	}

	/*
	 * Return a child node, or NULL if the child doesn't exist.
	 */
	auto get_child(this string_tree_node const &self,
		       std::string_view child)
		-> string_tree_node const *
	{
		if (auto it = self.children.find(std::string(child));
		    it != self.children.end())
			return &it->second;

		return nullptr;
	}

	auto get_child(this string_tree_node &self,
		       std::string_view child)
		-> string_tree_node *
	{
		if (auto it = self.children.find(std::string(child));
		    it != self.children.end())
			return &it->second;

		return nullptr;
	}

	/*
	 * Return a child node if it exists, or insert a new empty node.
	 */
	auto get_or_create_child(this string_tree_node &self,
				 std::string_view child)
		-> string_tree_node *
	{
		if (auto ptr = self.get_child(child); ptr != nullptr)
			return ptr;

		auto [it, ok] = self.children.emplace(
					child, string_tree_node(child));
		return &it->second;
	}

	/*
	 * Return this node's value.
	 */
	auto value(this string_tree_node const &self)
		-> std::optional<T> const &
	{
		return self._value;
	}

	/*
	 * Set this node's value.
	 */
	auto value(this string_tree_node &self, T value) -> void
	{
		self._value = std::move(value);
	}

private:
	std::string _this_word;
	std::optional<T> _value;
	std::unordered_map<std::string, string_tree_node> children;
};

template<typename T>
struct string_tree {
	/*
	 * Add a node to the tree.  Returns false if the node already exists.
	 */
	auto insert(this string_tree &self,
		    std::ranges::range auto &&path,
		    T value)
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
	auto find(this string_tree const &self,
		  std::ranges::range auto &&path)
		-> std::optional<T>
	{
		auto *this_node = &self._root_node;
{}
		// Find the node for this key.
		for (auto &&next : path) {
			this_node = this_node->get_child(next);

			if (this_node == nullptr)
				// The node doesn't exist.
				return {};

			if (this_node->value())
				// This node has a value, so return it.
				return {this_node->value()};
		}

		// We didn't find a value, so the key was incomplete.
		return {};
	}

private:
	string_tree_node<T> _root_node;
};

/*
 * The global command map.
 */
auto get_commands() -> string_tree<command_base *> & {
	static auto commands = string_tree<command_base *>();
	return commands;
}

auto register_command(std::string_view path, command_base *cmd) noexcept
	-> void
try {
{
	auto &commands = get_commands();
	if (commands.insert(path | std::views::split(' '), cmd) == false) {
		std::printf("command registration failed\n");
		std::abort();
	}
}
} catch (...) {
	std::printf("command registration failed\n");
	std::abort();
}


auto dispatch_command(int argc, char **argv) -> int
{
	if (argc == 0) {
		std::print("not enough arguments\n");
		return 1;
	}

	auto &commands = get_commands();
	auto node = commands.find(std::span(argv, argv + argc));
	if (node)
		return (**node).invoke(argc, argv);

	std::print("unknown command\n");
	return 1;
}

void print_usage(std::string_view)
{
//	get_root_node().print_usage(std::string(prefix));
}

} // namespace nihil
