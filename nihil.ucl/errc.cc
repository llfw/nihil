/*
 * This source code is released into the public domain.
 */

module;

#include <string>
#include <system_error>

module nihil.ucl;

import nihil;

namespace nihil::ucl {

struct ucl_error_category final : std::error_category {
	auto name() const noexcept -> char const * override;
	auto message(int err) const -> std::string override;
};

auto ucl_category() noexcept -> std::error_category &
{
	static auto category = ucl_error_category();
	return category;
}

auto make_error_condition(errc ec) -> std::error_condition
{
	return {static_cast<int>(ec), ucl_category()};
}

auto ucl_error_category::name() const noexcept -> char const *
{
	return "nihil.ucl";
}

auto ucl_error_category::message(int err) const -> std::string
{
	switch (static_cast<errc>(err)) {
	case errc::no_error:
		return "No error";
	case errc::failed_to_create_object:
		return "Failed to create UCL object";
	case errc::type_mismatch:
		return "UCL type does not match expected type";
	default:
		return "Undefined error";
	}
}

} // namespace nihil::ucl
