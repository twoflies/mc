// Plugin Command Interpreter

#include "plugincommandinterpreter.h"

PluginCommandInterpreter::PluginCommandInterpreter(Plugin* const plugin, OutputWriter* const outputWriter) : CommandInterpreter(outputWriter), plugin_(plugin) {
}

PluginCommandInterpreter::~PluginCommandInterpreter() {
}

Plugin* const PluginCommandInterpreter::getPlugin() {
  return plugin_;
}
