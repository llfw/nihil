/*
 * This source code is released into the public domain.
 */

module;

#include <string>
#include <system_error>

module nihil;

namespace nihil {

struct nihil_error_category final : std::error_category {
	auto name() const noexcept -> char const * override;
	auto message(int err) const -> std::string override;
};

auto nihil_category() noexcept -> std::error_category &
{
	static auto category = nihil_error_category();
	return category;
}

auto make_error_condition(errc ec) -> std::error_condition
{
	return {static_cast<int>(ec), nihil_category()};
}

auto nihil_error_category::name() const noexcept -> char const *
{
	return "nihil";
}

auto nihil_error_category::message(int err) const -> std::string
{
	switch (static_cast<errc>(err)) {
	case errc::no_error:
		return "No error";
	case errc::empty_string:
		return "Empty string is not permitted";
	case errc::invalid_unit:
		return "Invalid unit specifier";
	default:
		return "Undefined error";
	}
}

} // namespace nihil
