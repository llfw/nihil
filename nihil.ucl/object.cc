/*
 * This source code is released into the public domain.
 */

module;

#include <cstdlib>
#include <string>
#include <utility>

#include <ucl.h>

module nihil.ucl;

namespace nihil::ucl {

object::object(ref_t, ::ucl_object_t const *object)
	: _object(::ucl_object_ref(object))
{
}

object::object(noref_t, ::ucl_object_t *object)
	: _object(object)
{
}

object::~object() {
	if (_object != nullptr)
		::ucl_object_unref(_object);
}

object::object(object &&other) noexcept
	: _object(std::exchange(other._object, nullptr))
{}

object::object(object const &other) noexcept
	: _object(nullptr)
{
	*this = other;
}

auto object::operator=(this object &self, object &&other) noexcept
	-> object &
{
	if (&self != &other)
		self._object = std::exchange(other._object, nullptr);
	return self;
}

auto object::operator=(this object &self, object const &other)
	-> object &
{
	if (&self != &other) {
		auto *new_uobj = ::ucl_object_copy(other.get_ucl_object());
		if (new_uobj == nullptr)
			throw error("failed to copy UCL object");

		if (self._object != nullptr)
			::ucl_object_unref(self._object);
		self._object = new_uobj;
	}

	return self;
}

auto object::ref(this object const &self) -> object
{
	return object(nihil::ucl::ref, self.get_ucl_object());
}

auto object::type(this object const &self) -> object_type
{
	auto utype = ::ucl_object_type(self.get_ucl_object());
	return static_cast<object_type>(utype);
}

auto object::get_ucl_object(this object &self) -> ::ucl_object_t *
{
	if (self._object == nullptr)
		throw error("attempt to access empty UCL object");
	return self._object;
}

auto object::get_ucl_object(this object const &self) -> ::ucl_object_t const *
{
	if (self._object == nullptr)
		throw error("attempt to access empty UCL object");
	return self._object;
}

// Return the key of this object.
auto object::key(this object const &self) -> std::string_view
{
	auto dlen = std::size_t{};
	auto const *dptr = ::ucl_object_keyl(self.get_ucl_object(),
					     &dlen);
	return {dptr, dlen};
}

auto swap(object &a, object &b) -> void
{
	std::swap(a._object, b._object);
}

auto operator<=>(object const &lhs, object const &rhs) -> std::strong_ordering
{
	auto cmp = ::ucl_object_compare(lhs.get_ucl_object(),
					rhs.get_ucl_object());

	if (cmp < 0)
		return std::strong_ordering::less;
	else if (cmp > 0)
		return std::strong_ordering::greater;
	else
		return std::strong_ordering::equal;
}

auto operator==(object const &lhs, object const &rhs) -> bool
{
	return (lhs <=> rhs) == std::strong_ordering::equal;
}

} // namespace nihil::ucl
