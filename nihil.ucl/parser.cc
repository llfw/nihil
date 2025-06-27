/*
 * This source code is released into the public domain.
 */

module;

#include <expected>
#include <functional>
#include <string>

#include <ucl.h>

module nihil.ucl;

import nihil;

namespace nihil::ucl {

auto make_parser(int flags) -> std::expected<parser, error>
{
	auto *p = ::ucl_parser_new(flags);
	if (p != nullptr)
		return p;

	// TODO: Is there a way to get the actual error here?
	return std::unexpected(error("failed to create parser"));
}

auto macro_handler::handle(unsigned char const *data,
			   std::size_t len, void *ud)
	-> bool
{
	auto handler = static_cast<macro_handler *>(ud);
	auto string = std::string_view(
			reinterpret_cast<char const *>(data),
			len);
	return handler->callback(string);
}

parser::parser(::ucl_parser *uclp)
	: m_parser(uclp)
{
}

parser::~parser()
{
	if (m_parser)
		::ucl_parser_free(m_parser);
}

parser::parser(parser &&other) noexcept
	: m_parser(std::exchange(other.m_parser, nullptr))
	, m_macros(std::move(other.m_macros))
{
}

auto parser::operator=(this parser &self, parser &&other) noexcept
	-> parser &
{
	if (&self != &other) {
		if (self.m_parser)
			::ucl_parser_free(self.m_parser);

		self.m_parser = std::exchange(other.m_parser, nullptr);
		self.m_macros = std::move(other.m_macros);
	}

	return self;
}

auto parser::register_value(
	this parser &self,
	std::string_view variable,
	std::string_view value)
	-> void
{
	::ucl_parser_register_variable(
		self.get_parser(),
		std::string(variable).c_str(),
		std::string(value).c_str());
}

auto parser::top(this parser &self) -> map<object>
{
	auto obj = ::ucl_parser_get_object(self.get_parser());
	if (obj != nullptr)
		// ucl_parser_get_object() refs the object for us.
		return {noref, obj};

	throw std::logic_error(
		"attempt to call top() on an invalid ucl::parser");
}

auto parser::get_parser(this parser &self) -> ::ucl_parser *
{
	if (self.m_parser == nullptr)
		throw std::logic_error("attempt to fetch a null ucl::parser");
	
	return self.m_parser;
}

} // namespace nihil::ucl
