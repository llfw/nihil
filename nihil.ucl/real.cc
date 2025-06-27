/*
 * This source code is released into the public domain.
 */

module;

#include <cassert>
#include <compare>
#include <cstdlib>
#include <expected>
#include <string>
#include <system_error>

#include <ucl.h>

module nihil.ucl;

import nihil;

namespace nihil::ucl {

auto make_real(real::contained_type value)
	-> std::expected<real, error>
{
	auto *uobj = ::ucl_object_fromdouble(value);
	if (uobj == nullptr)
		return std::unexpected(error(
			errc::failed_to_create_object,
			error(std::errc(errno))));

	return real(noref, uobj);
}

real::real()
	: real(0)
{
}

real::real(contained_type value)
	: real(noref, [&] {
		auto *uobj = ::ucl_object_fromdouble(value);
		if (uobj == nullptr)
			throw std::system_error(
				std::make_error_code(std::errc(errno)));
		return uobj;
	}())
{
}

real::real(ref_t, ::ucl_object_t const *uobj)
	: object(nihil::ucl::ref, [&] {
		auto actual_type = static_cast<object_type>(
					::ucl_object_type(uobj));
		if (actual_type != real::ucl_type)
			throw type_mismatch(real::ucl_type, actual_type);
		return uobj;
	}())
{
}

real::real(noref_t, ::ucl_object_t *uobj)
	: object(nihil::ucl::noref, [&] {
		auto actual_type = static_cast<object_type>(
					::ucl_object_type(uobj));
		if (actual_type != real::ucl_type)
			throw type_mismatch(real::ucl_type, actual_type);
		return uobj;
	}())
{
}

auto real::value(this real const &self) -> contained_type
{
	auto v = contained_type{};
	auto const *uobj = self.get_ucl_object();

	if (::ucl_object_todouble_safe(uobj, &v))
		return v;

	std::abort();
}

auto operator== (real const &a, real const &b) -> bool
{
	return a.value() == b.value();
}

auto operator<=> (real const &a, real const &b) -> std::partial_ordering
{
	return a.value() <=> b.value();
}

auto operator== (real const &a, real::contained_type b) -> bool
{
	return a.value() == b;
}

auto operator<=> (real const &a, real::contained_type b)
	-> std::partial_ordering
{
	return a.value() <=> b;
}

} // namespace nihil::ucl
