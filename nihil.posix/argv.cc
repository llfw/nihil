/*
 * This source code is released into the public domain.
 */

module;

#include <memory>
#include <ranges>
#include <string>
#include <vector>

module nihil.posix;

namespace nihil {

argv::argv() = default;
argv::argv(argv &&) noexcept = default;
auto argv::operator=(this argv &, argv &&) -> argv & = default;

argv::~argv()
{
	for (auto *arg : m_args)
		delete[] arg;
}

auto argv::data(this argv const &self) -> char const * const *
{
	return self.m_args.data();
}

auto argv::data(this argv &self) -> char * const *
{
	return self.m_args.data();
}

auto argv::size(this argv const &self)
{
	return self.m_args.size();
}

auto argv::begin(this argv const &self)
{
	return self.m_args.begin();
}

auto argv::end(this argv const &self)
{
	return self.m_args.end();
}


auto argv::add_arg(this argv &self, std::string_view arg) -> void
{
	// Create a nul-terminated C string.
	auto ptr = std::make_unique<char[]>(arg.size() + 1);
	std::ranges::copy(arg, ptr.get());
	ptr[arg.size()] = '\0';

	// Ensure we won't throw when emplacing the pointer.
	self.m_args.reserve(self.m_args.size() + 1);
	self.m_args.emplace_back(ptr.release());
}

} // namespace nihil

