/*
 * This source code is released into the public domain.
 */

module;

#include <cassert>
#include <compare>
#include <cstdlib>
#include <string>

#include <ucl.h>

module nihil.ucl;

namespace nihil::ucl {

real::real(ref_t, ::ucl_object_t const *uobj)
	: object(nihil::ucl::ref, uobj)
{
	if (type() != ucl_type)
		throw type_mismatch(ucl_type, type());
}

real::real(noref_t, ::ucl_object_t *uobj)
	: object(noref, uobj)
{
	if (type() != ucl_type)
		throw type_mismatch(ucl_type, type());
}

real::real()
	: real(0)
{
}

real::real(contained_type value)
	: object(noref, ::ucl_object_fromdouble(value))
{
	if (_object == nullptr)
		throw error("failed to create UCL object");
}

auto real::value(this real const &self) -> contained_type
{
	auto v = contained_type{};
	auto const *uobj = self.get_ucl_object();

	if (::ucl_object_todouble_safe(uobj, &v))
		return v;

	std::abort();
}

auto operator== (real const &a, real const &b)
-> bool
{
	return a.value() == b.value();
}

auto operator<=> (real const &a, real const &b)
-> std::partial_ordering
{
	return a.value() <=> b.value();
}

auto operator== (real const &a, real::contained_type b)
-> bool
{
	return a.value() == b;
}

auto operator<=> (real const &a, real::contained_type b)
-> std::partial_ordering
{
	return a.value() <=> b;
}

} // namespace nihil::ucl
