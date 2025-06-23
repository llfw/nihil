/*
 * This source code is released into the public domain.
 */

module;

#include <format>
#include <string>

module nihil.config;

import nihil.ucl;

namespace nihil::config {

string::string(
	std::string &storage,
	std::string_view name,
	std::string_view description) noexcept
	: option(name, description)
	, _storage(storage)
{
	store::get().register_option(this);
}

string::~string()
{
	store::get().unregister_option(this);
}

auto string::get_string() const -> std::string
{
	return _storage;
}

auto string::set_string(std::string_view new_value) -> void
{
	_storage = new_value;
}

auto string::to_ucl() const -> ucl::object
{
	return ucl::string(_storage);
}

auto string::from_ucl(ucl::object const &uclobj) -> void
{
	try {
		_storage = object_cast<ucl::string>(uclobj).value();
		is_default(false);
	} catch (ucl::type_mismatch const &exc) {
		throw error(std::format("'{}': expected string, not {}",
			    name(), str(exc.actual_type())));
	}
}

} // namespace nihil::config
