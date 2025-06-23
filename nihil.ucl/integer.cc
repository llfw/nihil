/*
 * This source code is released into the public domain.
 */

module;

#include <compare>
#include <cstdlib>

#include <ucl.h>

module nihil.ucl;

namespace nihil::ucl {

integer::integer(ref_t, ::ucl_object_t const *uobj)
	: object(nihil::ucl::ref, uobj)
{
	if (type() != ucl_type)
		throw type_mismatch(ucl_type, type());
}

integer::integer(noref_t, ::ucl_object_t *uobj)
	: object(noref, uobj)
{
	if (type() != ucl_type)
		throw type_mismatch(ucl_type, type());
}

integer::integer()
	: integer(0)
{}

integer::integer(contained_type value)
	: object(noref, ::ucl_object_fromint(value))
{
	if (_object == nullptr)
		throw error("failed to create UCL object");
}

auto integer::value(this integer const &self) -> contained_type
{
	auto v = contained_type{};
	auto const *uobj = self.get_ucl_object();

	if (::ucl_object_toint_safe(uobj, &v))
		return v;

	std::abort();
}

auto operator== (integer const &a, integer const &b)
-> bool
{
	return a.value() == b.value();
}

auto operator<=> (integer const &a, integer const &b)
-> std::strong_ordering
{
	return a.value() <=> b.value();
}

auto operator== (integer const &a, integer::contained_type b)
-> bool
{
	return a.value() == b;
}

auto operator<=> (integer const &a, integer::contained_type b)
-> std::strong_ordering
{
	return a.value() <=> b;
}

} // namespace nihil::ucl
