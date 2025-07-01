// This source code is released into the public domain.
module nihil.cli;

import nihil.std;

namespace nihil {

/*
 * Get the registry storage.  Because this is called from global ctors,
 * it handles exceptions itself.
 */
auto get_registry() noexcept -> std::vector<std::shared_ptr<command_node>> &
try {
	static auto commands = std::vector<std::shared_ptr<command_node>>();
	return commands;
} catch (std::exception const &exc) {
	std::println(std::cerr, "{}", exc.what());
	std::exit(1); // NOLINT
} catch (...) {
	std::println(std::cerr, "get_registered_commands(): unknown error\n");
	std::exit(1); // NOLINT
}

/*
 * Register a new command.
 */
auto register_command(command *cmd) noexcept -> void
try {
	auto null_deleter = [] (command_node const *) -> void {};

	auto &commands = get_registry();
	commands.emplace_back(cmd, null_deleter);
} catch (std::exception const &exc) {
	std::println(std::cerr, "{}", exc.what());
	std::exit(1); // NOLINT
} catch (...) {
	std::println(std::cerr, "get_registered_commands(): unknown error\n");
	std::exit(1); // NOLINT
}

/*
 * Get the list of registered commands.
 */
auto get_registered_commands() -> std::span<std::shared_ptr<command_node>>
{
	return {get_registry()};
}

} // namespace nihil
