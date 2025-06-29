/*
 * This source code is released into the public domain.
 */

module;

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <variant>

module nihil.error;

import nihil.match;

namespace nihil {

auto to_string(error const &self) -> std::string
{
	auto ret = self.str();

	auto cause = self.cause();
	while (cause) {
		ret += ": " + cause->str();
		cause = cause->cause();
	}

	return ret;
}

error::error()
{
}

error::~error() = default;

error::error(std::string_view what, error cause)
	: m_error(std::string(what))
	, m_cause(std::make_shared<error>(std::move(cause)))
{
}
error::error(std::string_view what)
	: m_error(std::string(what))
{
}

error::error(std::error_condition what, error cause)
	: m_error(what)
	, m_cause(std::make_shared<error>(std::move(cause)))
{
}

error::error(std::error_condition what)
	: m_error(what)
{
}

error::error(std::error_code what, error cause)
	: m_error(what)
	, m_cause(std::make_shared<error>(std::move(cause)))
{
}

error::error(std::error_code what)
	: m_error(what)
{
}

error::error(error const &) = default;
error::error(error &&) noexcept = default;
auto error::operator=(this error &, error const &) -> error & = default;
auto error::operator=(this error &, error &&) noexcept -> error & = default;

auto error::cause(this error const &self) -> std::shared_ptr<error>
{
	if (self.m_cause)
		return self.m_cause;
	return {};
}

auto error::root_cause(this error const &self) -> error const &
{
	if (self.m_cause)
		return self.m_cause->root_cause();

	return self; //NOLINT(bugprone-return-const-ref-from-parameter)
}

auto error::str(this error const &self) -> std::string
{
	return self.m_error | match {
		[] (std::monostate) -> std::string {
			return "No error";
		},
		[] (std::error_code const &m) {
			return m.message();
		},
		[] (std::error_condition const &m) {
			return m.message();
		},
		[] (std::string const &m) {
			return m;
		}
	};
}

auto error::code(this error const &self) -> std::optional<std::error_code>
{
	auto const *code = std::get_if<std::error_code>(&self.m_error);
	if (code)
		return {*code};
	return {};
}

auto error::condition(this error const &self)
	-> std::optional<std::error_condition>
{
	auto const *condition = std::get_if<std::error_condition>(&self.m_error);
	if (condition)
		return {*condition};
	return {};
}

auto error::what() const noexcept -> char const *
{
	if (!m_what)
		m_what = to_string(*this);

	return m_what->c_str();
}

auto operator==(error const &lhs, error const &rhs) -> bool
{
	return lhs.m_error == rhs.m_error;
}

auto operator<=>(error const &lhs, error const &rhs) -> std::strong_ordering
{
	return lhs.m_error <=> rhs.m_error;
}

auto operator==(error const &lhs, std::error_code const &rhs) -> bool
{
	return lhs.code() == rhs;
}

// Compare an error to an std::error_condition.
auto operator==(error const &lhs, std::error_condition const &rhs) -> bool
{
	return lhs.condition() == rhs;
}

auto operator<<(std::ostream &strm, error const &e) -> std::ostream &
{
	return strm << to_string(e);
}

} // namespace nihil
