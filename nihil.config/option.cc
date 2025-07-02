// This source code is released into the public domain.
module nihil.config;

import nihil.std;
import nihil.ucl;
import nihil.util;

namespace nihil::config {

//NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
option::option(std::string_view const name, std::string_view const description)
	: m_name(name)
	, m_description(description)
{
	auto okay = store::get().register_option(this);
	if (okay)
		return;

	std::println(std::cerr,
		   "INTERNAL ERROR: failed to register "
		   "configuration option '{}': {}",
		   m_name, okay.error());
	std::exit(1); // NOLINT
}

option::~option()
{
	std::ignore = store::get().unregister_option(this);
}

auto option::name(this option const &self) noexcept
	-> std::string_view
{
	return self.m_name;
}

auto option::description(this option const &self) noexcept
	-> std::string_view
{
	return self.m_description;
}

auto option::is_default(this option const &self) noexcept
	-> bool
{
	return self.m_is_default;
}

auto option::is_default(this option  &self, bool b) -> void
{
	self.m_is_default = b;
}

auto option::string(this option const &self) -> std::string
{
	return self.get_string();
}

auto option::string(this option &self, std::string_view value)
	-> std::expected<void, error>
{
	co_await self.set_string(value);
	self.is_default(false);
	co_return {};
}

auto option::ucl(this option const &self)
	-> std::expected<nihil::ucl::object, error>
{
	return self.get_ucl();
}

auto option::ucl(this option &self, nihil::ucl::object const &value)
	-> std::expected<void, error>
{
	co_await self.set_ucl(value);
	self.is_default(false);
	co_return {};
}

auto operator<<(std::ostream &strm, option const &opt)
-> std::ostream &
{
	return strm << "<" << opt.name() << "=" << opt.string() << ">";
}

} // namespace nihil
