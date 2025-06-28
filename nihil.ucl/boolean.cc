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

import nihil.error;

namespace nihil::ucl {

auto make_boolean(boolean::contained_type value)
	-> std::expected<boolean, error>
{
	auto *uobj = ::ucl_object_frombool(value);
	if (uobj == nullptr)
		return std::unexpected(error(
			errc::failed_to_create_object,
			error(std::errc(errno))));

	return boolean(noref, uobj);
}

boolean::boolean()
	: boolean(false)
{
}

boolean::boolean(contained_type value)
	: object(noref, [&] {
		auto *uobj = ::ucl_object_frombool(value);
		if (uobj == nullptr)
			throw std::system_error(
				std::make_error_code(std::errc(errno)));
		return uobj;
	}())
{
}

boolean::boolean(ref_t, ::ucl_object_t const *uobj)
	: object(nihil::ucl::ref, [&] {
		auto actual_type = static_cast<object_type>(
					::ucl_object_type(uobj));
		if (actual_type != boolean::ucl_type)
			throw type_mismatch(boolean::ucl_type, actual_type);
		return uobj;
	}())
{
}

boolean::boolean(noref_t, ::ucl_object_t *uobj)
	: object(nihil::ucl::noref, [&] {
		auto actual_type = static_cast<object_type>(
					::ucl_object_type(uobj));
		if (actual_type != boolean::ucl_type)
			throw type_mismatch(boolean::ucl_type, actual_type);
		return uobj;
	}())
{
}

auto boolean::value(this boolean const &self)
	-> contained_type
{
	auto v = contained_type{};
	auto const *uobj = self.get_ucl_object();

	if (::ucl_object_toboolean_safe(uobj, &v))
		return v;

	std::abort();
}

auto operator== (boolean const &a, boolean const &b)
	-> bool
{
	return a.value() == b.value();
}

auto operator<=> (boolean const &a, boolean const &b)
	-> std::strong_ordering
{
	return a.value() <=> b.value();
}

auto operator== (boolean const &a, boolean::contained_type b)
	-> bool
{
	return a.value() == b;
}

auto operator<=> (boolean const &a, boolean::contained_type b)
	-> std::strong_ordering
{
	return a.value() <=> b;
}

} // namespace nihil::ucl
