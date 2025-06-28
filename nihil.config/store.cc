/*
 * This source code is released into the public domain.
 */

module;

#include <coroutine>
#include <expected>
#include <filesystem>
#include <format>
#include <map>

module nihil.config;

import nihil.error;
import nihil.generator;
import nihil.monad;

namespace nihil::config {

store::store() = default;

auto store::get() -> store &
{
	static auto instance = store();
	return instance;
}


auto store::register_option(this store &self, option *object)
	-> std::expected<void, error>
{
	auto [it, okay] = self.m_options.insert(
				std::pair{object->name(), object});

	if (okay)
		return {};

	return std::unexpected(error(std::format(
			"attempt to register duplicate "
			"configuration option '{0}'",
			object->name())));
}

auto store::unregister_option(this store &self, option *object)
	-> std::expected<void, error>
{
	auto it = self.m_options.find(object->name());
	if (it == self.m_options.end())
		return std::unexpected(error(std::format(
			"attempt to unregister non-existent "
			"configuration option '{}'",
			object->name())));

	self.m_options.erase(it);
	return {};
}

auto store::fetch(this store const &self, std::string_view name)
	-> std::expected<option const *, error>
{
	if (auto it = self.m_options.find(name); it != self.m_options.end())
		return it->second;

	return std::unexpected(error(std::format(
		"unknown configuration option '{}'",
		name)));
}

auto store::fetch(this store &self, std::string_view name)
	-> std::expected<option *, error>
{
	auto opt = co_await static_cast<store const &>(self).fetch(name);
	co_return const_cast<option *>(opt);
}

auto store::all(this store const &self) -> nihil::generator<option const *>
{
	for (auto &&it : self.m_options)
		co_yield it.second;
}

auto store::all(this store &self) -> nihil::generator<option *>
{
	for (auto &&it : self.m_options)
		co_yield it.second;
}

auto get_option(std::string_view option_name)
	-> std::expected<option *, error>
{
	co_return co_await store::get().fetch(option_name);
}

} // namespace nihil::config
