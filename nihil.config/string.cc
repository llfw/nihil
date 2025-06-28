/*
 * This source code is released into the public domain.
 */

module;

#include <coroutine>
#include <expected>
#include <format>
#include <string>

module nihil.config;

import nihil.error;
import nihil.monad;
import nihil.ucl;

namespace nihil::config {

string::string(
	std::string &storage,
	std::string_view name,
	std::string_view description) noexcept
	: option(name, description)
	, m_storage(storage)
{
}

string::~string() = default;

auto string::get_string() const -> std::string
{
	return m_storage;
}

auto string::set_string(std::string_view new_value)
	-> std::expected<void, error>
{
	m_storage = new_value;
	return {};
}

auto string::get_ucl() const -> std::expected<ucl::object, error>
{
	return ucl::string(m_storage);
}

auto string::set_ucl(ucl::object const &uclobj) -> std::expected<void, error>
{
	auto obj = co_await object_cast<ucl::string>(uclobj)
		.transform_error([&] (ucl::type_mismatch const &m) {
			return error(std::format(
				"'{}': expected string, not {}",
				name(), str(m.actual_type())));
		});

	m_storage = obj.value();
	is_default(false);
	co_return {};
}

} // namespace nihil::config
