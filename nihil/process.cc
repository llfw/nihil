/*
 * This source code is released into the public domain.
 */

module;

#include <cerrno>
#include <cstring>
#include <format>
#include <optional>
#include <system_error>
#include <utility>

#include <sys/types.h>
#include <sys/wait.h>

module nihil;

namespace nihil {

process_error::process_error(std::string what)
	: generic_error(std::move(what))
{
}

waitpid_error::waitpid_error(::pid_t pid, std::error_code error)
	: process_error(std::format("waitpid({}): {}",
				    pid, error.message()))
	, _pid(pid)
	, _error(error)
{
}

auto waitpid_error::pid(this waitpid_error const &self) -> ::pid_t
{
	return self._pid;
}

auto waitpid_error::error(this waitpid_error const &self) -> std::error_code
{
	return self._error;
}

auto wait_result::okay(this wait_result const &self) -> bool
{
	return self.status() == 0;
}

wait_result::operator bool(this wait_result const &self)
{
	return self.okay();
}

auto wait_result::status(this wait_result const &self) -> std::optional<int>
{
	if (WIFEXITED(self._status))
		return WEXITSTATUS(self._status);
	return {};
}

auto wait_result::signal(this wait_result const &self) -> std::optional<int>
{
	if (WIFSIGNALED(self._status))
		return WTERMSIG(self._status);
	return {};
}

wait_result::wait_result(int status)
	: _status(status)
{}

process::process(::pid_t pid)
	: _pid(pid)
{}

process::~process() {
	if (_pid == -1)
		return;

	auto status = int{};
	std::ignore = waitpid(_pid, &status, WEXITED);
}

process::process(process &&) noexcept = default;
auto process::operator=(process &&) noexcept -> process& = default;

// Get the child's process id.
auto process::pid(this process const &self) noexcept -> ::pid_t
{
	return self._pid;
}

auto process::wait(this process &&self) -> wait_result
{
	auto status = int{};
	auto ret = waitpid(self._pid, &status, WEXITED);
	if (ret != -1)
		return wait_result(status);

	throw waitpid_error(self._pid,
			    std::make_error_code(std::errc(errno)));
}

auto process::release(this process &&self) -> ::pid_t
{
	auto const ret = self.pid();
	self._pid = -1;
	return ret;
}

} // namespace nihil
