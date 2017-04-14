#ifndef PLUGINCOMMANDINTERPRETER_H_
#define PLUGINCOMMANDINTERPRETER_H_

// Plugin Command Interpreter

#include <string>
#include <vector>

#include "plugin.h"
#include "commandinterpreter.h"

class PluginCommandInterpreter : public CommandInterpreter {
 public:
  PluginCommandInterpreter(Plugin* const plugin, OutputWriter* const outputWriter);
  virtual ~PluginCommandInterpreter();
  Plugin* const getPlugin();

 private:
  Plugin* const plugin_;
};

#endif // PLUGINCOMMANDINTERPRETER_H_
