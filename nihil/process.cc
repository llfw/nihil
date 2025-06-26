/*
 * This source code is released into the public domain.
 */

module;

#include <cerrno>
#include <cstring>
#include <expected>
#include <format>
#include <optional>
#include <system_error>
#include <utility>

#include <sys/types.h>
#include <sys/wait.h>

module nihil;

namespace nihil {

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
	: m_pid(pid)
{}

process::~process() {
	if (m_pid == -1)
		return;

	auto status = int{};
	std::ignore = waitpid(m_pid, &status, WEXITED);
}

process::process(process &&other) noexcept
	: m_pid(std::exchange(other.m_pid, -1))
{
}

auto process::operator=(this process &self, process &&other) noexcept
	-> process &
{
	if (&self != &other) {
		self.m_pid = std::exchange(other.m_pid, -1);
	}

	return self;
}

// Get the child's process id.
auto process::pid(this process const &self) noexcept -> ::pid_t
{
	return self.m_pid;
}

auto process::wait(this process &&self) -> std::expected<wait_result, error>
{
	auto status = int{};
	auto ret = waitpid(self.m_pid, &status, WEXITED);
	if (ret == -1)
		return std::unexpected(error(std::errc(errno)));

	return wait_result(status);
}

auto process::release(this process &&self) -> ::pid_t
{
	auto const ret = self.pid();
	self.m_pid = -1;
	return ret;
}

} // namespace nihil
