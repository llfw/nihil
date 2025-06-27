/*
 * This source code is released into the public domain.
 */

module;

#include <format>

module nihil.ucl;

namespace nihil::ucl {

auto str(object_type type) -> std::string_view {
	using namespace std::literals;

	switch (type) {
	case object_type::object:
		return "object"sv;
	case object_type::array:
		return "array"sv;
	case object_type::integer:
		return "integer"sv;
	case object_type::real:
		return "real"sv;
	case object_type::string:
		return "string"sv;
	case object_type::boolean:
		return "boolean"sv;
	case object_type::time:
		return "time"sv;
	case object_type::userdata:
		return "userdata"sv;
	case object_type::null:
		return "null"sv;
	default:
		// Don't fail here, since UCL might add more types that we
		// don't know about.
		return "unknown"sv;
	}
}

type_mismatch::type_mismatch(object_type expected_type,
			     object_type actual_type)
	: error(std::format(
		"expected type '{}' != actual type '{}'",
		ucl::str(expected_type), ucl::str(actual_type)))
	, m_expected_type(expected_type)
	, m_actual_type(actual_type)
{
}

auto type_mismatch::expected_type(this type_mismatch const &self) -> object_type
{
	return self.m_expected_type;
}

auto type_mismatch::actual_type(this type_mismatch const &self) -> object_type
{
	return self.m_actual_type;
}
	
} // namespace nihil::ucl
