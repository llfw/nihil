/*
 * This source code is released into the public domain.
 */

module;

#include <functional>
#include <string>

#include <ucl.h>

module nihil.ucl;

namespace nihil::ucl {

parse_error::parse_error(std::string what)
	: error(std::move(what))
{
}

auto macro_handler::handle(unsigned char const *data, std::size_t len, void *ud)
-> bool
{
	auto handler = static_cast<macro_handler *>(ud);
	auto string = std::string_view(
			reinterpret_cast<char const *>(data),
			len);
	return handler->callback(string);
}

parser::parser(int flags)
{
	if ((_parser = ::ucl_parser_new(flags)) != nullptr)
		return;

	throw error("failed to create UCL parser");
}

parser::parser()
	: parser(0)
{
}

parser::~parser()
{
	if (_parser)
		::ucl_parser_free(_parser);
}

auto parser::register_value(
	this parser &self,
	std::string_view variable,
	std::string_view value)
-> void
{
	::ucl_parser_register_variable(
		self._parser,
		std::string(variable).c_str(),
		std::string(value).c_str());
}

auto parser::top(this parser &self) -> map<object>
{
	if (self._parser == nullptr)
		throw error("attempt to call top() on an empty parser");

	auto obj = ::ucl_parser_get_object(self._parser);
	if (obj == nullptr)
		throw error("attempt to call top() on an empty parser");

	// ucl_parser_get_objects() refs the object for us.
	return {noref, obj};
}

} // namespace nihil::ucl
