/*
 * This source code is released into the public domain.
 */

module;

#include <cstdlib>
#include <expected>
#include <iosfwd>
#include <string>
#include <system_error>

#include <ucl.h>

module nihil.ucl;

import nihil.error;

namespace nihil::ucl {

auto make_string() -> std::expected<string, error>
{
	return make_string(std::string_view(""));
}

auto make_string(char const *s) -> std::expected<string, error>
{
	return make_string(std::string_view(s));
}

auto make_string(std::string_view s) -> std::expected<string, error>
{
	auto *uobj = ::ucl_object_fromstring_common(
			s.data(), s.size(), UCL_STRING_RAW);

	if (uobj == nullptr)
		return std::unexpected(error(
			errc::failed_to_create_object,
			error(std::errc(errno))));

	return string(noref, uobj);
}

string::string(ref_t, ::ucl_object_t const *uobj)
	: object(nihil::ucl::ref, [&] {
		auto actual_type = static_cast<object_type>(
					::ucl_object_type(uobj));
		if (actual_type != string::ucl_type)
			throw type_mismatch(string::ucl_type, actual_type);
		return uobj;
	}())
{
}

string::string(noref_t, ::ucl_object_t *uobj)
	: object(nihil::ucl::noref, [&] {
		auto actual_type = static_cast<object_type>(
					::ucl_object_type(uobj));
		if (actual_type != string::ucl_type)
			throw type_mismatch(string::ucl_type, actual_type);
		return uobj;
	}())
{
}

string::string()
	: string(std::string_view(""))
{}

string::string(std::string_view value)
	: string(noref, [&] {
		auto *uobj = ::ucl_object_fromstring_common(
				value.data(), value.size(), UCL_STRING_RAW);
		if (uobj == nullptr)
			throw std::system_error(
				std::make_error_code(std::errc(errno)));
		return uobj;
	}())
{
}

string::string(char const *value)
	: string(std::string_view(value))
{
}

auto string::value(this string const &self) -> contained_type
{
	char const *dptr{};
	std::size_t dlen;

	auto const *uobj = self.get_ucl_object();
	if (::ucl_object_tolstring_safe(uobj, &dptr, &dlen))
		return {dptr, dlen};

	// This should never fail.
	std::abort();
}

auto string::size(this string const &self) -> size_type
{
	return self.value().size();
}

auto string::empty(this string const &self) -> bool
{
	return self.size() == 0;
}

auto string::data(this string const &self) -> pointer
{
	char const *dptr{};

	auto const *uobj = self.get_ucl_object();
	if (::ucl_object_tostring_safe(uobj, &dptr))
		return dptr;

	// This should never fail.
	std::abort();
}

auto string::begin(this string const &self) -> iterator
{
	return self.data();
}

auto string::end(this string const &self) -> iterator
{
	return self.data() + self.size();
}

auto operator== (string const &a, string const &b)
	-> bool
{
	return a.value() == b.value();
}

auto operator<=> (string const &a, string const &b)
	-> std::strong_ordering
{
	return a.value() <=> b.value();
}

/*
 * For convenience, allow comparison with C++ strings without having to
 * construct a temporary UCL object.
 */

auto operator==(string const &lhs, std::string_view rhs) -> bool
{
	return lhs.value() == rhs;
}

auto operator<=>(string const &lhs, std::string_view rhs)
	-> std::strong_ordering
{
	return lhs.value() <=> rhs;
}

auto operator==(string const &lhs, std::string const &rhs) -> bool
{
	return lhs == std::string_view(rhs);
}

auto operator<=>(string const &lhs, std::string const &rhs)
	-> std::strong_ordering
{
	return lhs <=> std::string_view(rhs);
}

auto operator==(string const &lhs, char const *rhs) -> bool
{
	return lhs == std::string_view(rhs);
}

auto operator<=>(string const &lhs, char const *rhs)
	-> std::strong_ordering
{
	return lhs <=> std::string_view(rhs);
}

auto operator<<(std::ostream &strm, string const &s) -> std::ostream &
{
	return strm << s.value();
}

} // namespace nihil::ucl
