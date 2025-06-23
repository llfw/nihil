/*
 * This source code is released into the public domain.
 */

module;

#include <coroutine>
#include <filesystem>
#include <format>
#include <map>

module nihil.config;

import nihil;

namespace nihil::config {

unknown_option::unknown_option(std::string_view option_name)
	: error(std::format("unknown configuration variable '{}'",
			    option_name))
	, _option_name(option_name)
{
}

auto unknown_option::option_name(this unknown_option const &self)
-> std::string_view
{
	return self._option_name;
}

auto store::get()
-> store&
{
	if (instance == nullptr)
		instance = new store;

	return *instance;
}


auto store::register_option(this store &self, option *object)
-> void
{
	auto [it, okay] = self.options.insert(
				std::pair{object->name(), object});

	if (!okay)
		throw error(std::format(
			"INTERNAL ERROR: attempt to register "
			"duplicate config value '{0}'",
			object->name()));
}

auto store::unregister_option(this store &self, option *object)
-> void
{
	auto it = self.options.find(object->name());
	if (it == self.options.end())
		throw error(std::format(
			"INTERNAL ERROR: attempt to unregister "
			"non-existent config value '{}'",
			object->name()));

	self.options.erase(it);
}

auto store::fetch(this store const &self, std::string_view name)
-> option &
{
	if (auto it = self.options.find(name); it != self.options.end())
		return *it->second;

	throw unknown_option(name);
}

auto store::all(this store const &self)
-> nihil::generator<option const &>
{
	for (auto &&it : self.options)
		co_yield *it.second;
}

auto get_option(std::string_view option_name)
-> option &
{
	return store::get().fetch(option_name);
}

} // namespace nihil::config
