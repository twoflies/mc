// Plugin Command Interpreter

#include "plugincommandinterpreter.h"

PluginCommandInterpreter::PluginCommandInterpreter(Plugin *plugin) {
  plugin_ = plugin;
}

PluginCommandInterpreter::~PluginCommandInterpreter() {
}

Plugin *PluginCommandInterpreter::getPlugin() {
  return plugin_;
}
