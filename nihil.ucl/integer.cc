/*
 * This source code is released into the public domain.
 */

module;

#include <compare>
#include <cstdlib>
#include <expected>
#include <system_error>

#include <ucl.h>

module nihil.ucl;

import nihil;

namespace nihil::ucl {

integer::integer()
	: integer(0)
{
}

integer::integer(contained_type value)
	: integer(noref, [&] {
		auto *uobj = ::ucl_object_fromint(value);
		if (uobj == nullptr)
			throw std::system_error(
				std::make_error_code(std::errc(errno)));
		return uobj;
	}())
{
}

integer::integer(ref_t, ::ucl_object_t const *uobj)
	: object(nihil::ucl::ref, [&] {
		auto actual_type = static_cast<object_type>(
					::ucl_object_type(uobj));
		if (actual_type != integer::ucl_type)
			throw type_mismatch(integer::ucl_type, actual_type);
		return uobj;
	}())
{
}

integer::integer(noref_t, ::ucl_object_t *uobj)
	: object(nihil::ucl::noref, [&] {
		auto actual_type = static_cast<object_type>(
					::ucl_object_type(uobj));
		if (actual_type != integer::ucl_type)
			throw type_mismatch(integer::ucl_type, actual_type);
		return uobj;
	}())
{
}

auto make_integer(integer::contained_type value)
	-> std::expected<integer, error>
{
	auto *uobj = ::ucl_object_fromint(value);
	if (uobj == nullptr)
		return std::unexpected(error(
			errc::failed_to_create_object,
			error(std::errc(errno))));

	return integer(noref, uobj);
}

auto integer::value(this integer const &self) -> contained_type
{
	auto v = contained_type{};
	auto const *uobj = self.get_ucl_object();

	if (::ucl_object_toint_safe(uobj, &v))
		return v;

	std::abort();
}

auto operator== (integer const &a, integer const &b) -> bool
{
	return a.value() == b.value();
}

auto operator<=> (integer const &a, integer const &b) -> std::strong_ordering
{
	return a.value() <=> b.value();
}

auto operator== (integer const &a, integer::contained_type b) -> bool
{
	return a.value() == b;
}

auto operator<=> (integer const &a, integer::contained_type b)
	-> std::strong_ordering
{
	return a.value() <=> b;
}

} // namespace nihil::ucl
