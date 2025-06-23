/*
 * This source code is released into the public domain.
 */

module;

#include <iostream>
#include <iterator>

module nihil.ucl;

namespace nihil::ucl {

auto operator<<(std::ostream &stream, object const &o)
-> std::ostream &
{
	emit(o, emitter::json, std::ostream_iterator<char>(stream));
	return stream;
}

} // namespace nihil::ucl
