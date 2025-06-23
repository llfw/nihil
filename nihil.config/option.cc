/*
 * This source code is released into the public domain.
 */

module;

#include <iostream>
#include <string>

module nihil.config;

namespace nihil::config {

auto option::name(this option const &self) noexcept
-> std::string_view
{
	return self._name;
}

// Human-readable description of this option.
auto option::description(this option const &self) noexcept
-> std::string_view
{
	return self._description;
}

// If true, this option is set to its default value.
auto option::is_default(this option const &self) noexcept
-> bool
{
	return self._is_default;
}

// Get or set this option as a string.
auto option::string(this option const &self)
-> std::string
{
	return self.get_string();
}

auto option::string(this option &self, std::string_view value)
-> void
{
	self.set_string(value);
	self._is_default = false;
}

option::option(std::string_view name,
       std::string_view description)
	: _name(name)
	, _description(description)
{
}

auto option::is_default(bool b)
-> void
{
	_is_default = b;
}

auto operator<<(std::ostream &strm, option const &opt)
-> std::ostream &
{
	return strm << "<" << opt.name() << "=" << opt.string() << ">";
}

} // namespace nihil
