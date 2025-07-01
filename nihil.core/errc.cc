// This source code is released into the public domain.
module nihil.core;

import nihil.std;

namespace nihil {

struct nihil_error_category final : std::error_category {
	[[nodiscard]] auto name() const noexcept -> char const * override;
	[[nodiscard]] auto message(int err) const -> std::string override;
};

[[nodiscard]] auto nihil_category() noexcept -> std::error_category &
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
	case errc::incomplete_command:
		return "Incomplete command";
	case errc::empty_string:
		return "Empty string is not permitted";
	case errc::invalid_unit:
		return "Invalid unit specifier";
	case errc::failed_to_create_object:
		return "Failed to create UCL object";
	case errc::type_mismatch:
		return "UCL type does not match expected type";
	default:
		return "Undefined error";
	}
}

} // namespace nihil
