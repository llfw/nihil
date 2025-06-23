/*
 * This source code is released into the public domain.
 */

module;

#include <cstdlib>
#include <string>

#include <ucl.h>

module nihil.ucl;

namespace nihil::ucl {

string::string(ref_t, ::ucl_object_t const *uobj)
	: object(nihil::ucl::ref, uobj)
{
	if (type() != ucl_type)
		throw type_mismatch(ucl_type, type());
}

string::string(noref_t, ::ucl_object_t *uobj)
	: object(noref, uobj)
{
	if (type() != ucl_type)
		throw type_mismatch(ucl_type, type());
}

string::string()
	: string(std::string_view(""))
{}

string::string(std::string_view value)
	: object(nihil::ucl::ref, 
		 ::ucl_object_fromstring_common(
			value.data(), value.size(),
			UCL_STRING_RAW))
{
	if (_object == nullptr)
		throw error("failed to create UCL object");
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

} // namespace nihil::ucl
