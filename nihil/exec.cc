/*
 * This source code is released into the public domain.
 */

module;

#include <format>
#include <string>
#include <utility>

#include <err.h>
#include <fcntl.h>
#include <unistd.h>

extern char **environ;

module nihil;

namespace nihil {

exec_error::exec_error(std::string what)
	: generic_error(std::move(what))
{}

executable_not_found::executable_not_found(std::string_view filename)
	: exec_error(std::format("{}: command not found", filename))
	, _filename(filename)
{
}

auto executable_not_found::filename(this executable_not_found const &self)
-> std::string_view
{
	return self._filename;
}

fexecv::fexecv(fd &&execfd, argv &&args) noexcept
	: _execfd(std::move(execfd))
	, _args(std::move(args))
{
}

auto fexecv::exec(this fexecv &self) noexcept 
-> void
{
	::fexecve(self._execfd.get(), self._args.data(), environ);
	::err(1, "fexecve");
}
	
fexecv::fexecv(fexecv &&) noexcept = default;
auto fexecv::operator=(this fexecv &, fexecv &&) noexcept -> fexecv& = default;
	
auto execv(std::string_view path, argv &&argv) -> fexecv
{
	auto cpath = std::string(path);
	auto const ret = ::open(cpath.c_str(), O_EXEC);
	if (ret == -1)
		throw executable_not_found(path);
	return {fd(ret), std::move(argv)};
}

auto execvp(std::string_view file, argv &&argv) -> fexecv
{
	auto execfd = find_in_path(file);
	if (!execfd)
		throw executable_not_found(file);
	return {std::move(*execfd), std::move(argv)};
}

auto shell(std::string_view const &command) -> fexecv
{
	return execl("/bin/sh", "sh", "-c", command);
}

} // namespace nihil
