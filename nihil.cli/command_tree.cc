/*
 * This source code is released into the public domain.
 */

module;

#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <vector>

module nihil.cli;

namespace nihil {

command_tree_node::command_tree_node()
	: m_this_word("")
	, m_command(std::make_shared<command_node>(command_node("")))
{
}

command_tree_node::command_tree_node(command_tree_node *parent,
				     std::string_view this_word)
	: m_parent(parent)
	, m_this_word(this_word)
{
}

command_tree_node::command_tree_node(command_tree_node *parent,
				     std::string_view this_word,
				     std::shared_ptr<command_node> command)
	: m_parent(parent)
	, m_this_word(this_word)
	, m_command(std::move(command))
{
}

auto command_tree_node::get_child(this command_tree_node const &self,
			     std::string_view child)
	-> command_tree_node const *
{
	if (auto it = self.m_children.find(std::string(child));
	    it != self.m_children.end())
		return &it->second;

	return nullptr;
}

auto command_tree_node::get_child(this command_tree_node &self,
			     std::string_view child)
	-> command_tree_node *
{
	if (auto it = self.m_children.find(std::string(child));
	    it != self.m_children.end())
		return &it->second;

	return nullptr;
}

auto command_tree_node::get_or_create_child(this command_tree_node &self,
				       std::string_view child)
	-> command_tree_node *
{
	// Return the existing child, if there is one.
	if (auto ptr = self.get_child(child); ptr != nullptr)
		return ptr;

	// Insert a new child.
	auto [it, ok] = self.m_children.emplace(
				child,
				command_tree_node(&self, child));

	// Give the child a dummy command.
	it->second.m_command = std::make_shared<command_node>(
				self.path() + ' ' + child);

	return &it->second;
}

auto command_tree_node::command(this command_tree_node const &self)
	-> std::shared_ptr<command_node> const &
{
	return self.m_command;
}

auto command_tree_node::command(this command_tree_node &self,
				std::shared_ptr<command_node> command)
	-> void
{
	// TODO: Put this check back without tripping from the dummy command.
	//if (self.m_command != nullptr)
	//	throw std::logic_error("duplicate command");
	self.m_command = std::move(command);
}

auto command_tree_node::print_commands(this command_tree_node const &self)
	-> void
{
	auto prefix = std::string(self.path());

	for (auto &&[name, node] : self.m_children) {
		auto command = prefix.empty()
			? name
			: (prefix + ' ' + name);
		std::print(std::cerr, "  {}\n", command);
	}
}

auto command_tree_node::path(this command_tree_node const &self)
	-> std::string
{
	auto path = std::string();

	auto const *node = &self;
	while (node->m_parent != nullptr) {
		path = node->m_this_word + ' ' + path;
		node = node->m_parent;
	}

	// Trim the trailing space.
	if (!path.empty())
		path.pop_back();

	return path;
}

auto command_tree::insert(this command_tree &self,
	    std::vector<std::string_view> const &path,
	    std::shared_ptr<command_node> command)
	-> void
{
	auto *this_node = &self.m_root_node;

	// Find the node for this key.
	for (auto &&this_word : path)
		this_node = this_node->get_or_create_child(this_word);

	// Set the new value.
	this_node->command(std::move(command));
}

auto command_tree::find(this command_tree const &self, int &argc, char **&argv)
	-> command_tree_node const *
{
	auto *this_node = &self.m_root_node;

	// Iterate until we don't find a child command, then return that node.
	while (argv[0] != nullptr) {
		auto *next_node = this_node->get_child(argv[0]);

		if (next_node == nullptr)
			return this_node;

		this_node = next_node;

		--argc;
		++argv;
	}

	// We ran out of path without finding a valid command.  Return this
	// node; the caller will notice the missing command.
	return this_node;
}

auto build_command_tree() -> command_tree
{
	auto const &commands = get_registered_commands();
	auto tree = command_tree();

	for (auto &&command : commands) {
		auto split_path = std::vector<std::string_view>(
			std::from_range,
			command->path()
			| std::views::split(' ')
			| std::views::transform([] (auto &&r) {
				return std::string_view(r);
			}));

		// Throws std::logic_error on duplicates.
		tree.insert(split_path, command);
	}

	return tree;
}

} // namespace nihil
