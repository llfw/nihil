/*
 * This source code is released into the public domain.
 */

module;

#include <string>
#include <utility>

module nihil.ucl;

namespace nihil::ucl {

error::error(std::string what)
	: generic_error(std::move(what))
{
}

} // namespace nihil::ucl
