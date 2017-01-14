#ifndef PLUGINCOMMANDINTERPRETER_H_
#define PLUGINCOMMANDINTERPRETER_H_

// Plugin Command Interpreter

#include <string>
#include <vector>

#include "plugin.h"
#include "commandinterpreter.h"

class PluginCommandInterpreter : public CommandInterpreter {
 public:
  PluginCommandInterpreter(Plugin *plugin);
  virtual ~PluginCommandInterpreter();
  Plugin *getPlugin();

 private:
  Plugin *plugin_;
};

#endif // PLUGINCOMMANDINTERPRETER_H_
