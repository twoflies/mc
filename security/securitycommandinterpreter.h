#ifndef SECURITYCOMMANDINTERPRETER_H_
#define SECURITYCOMMANDINTERPRETER_H_

// Security Command Interpreter

#include <string>
#include <map>

#include "../plugincommandinterpreter.h"

class SecurityCommandInterpreter : public PluginCommandInterpreter {
  typedef int (SecurityCommandInterpreter::*CommandFunction)(const std::string&, const std::vector<std::string> &, CommandOutputWriter*);

 public:
  SecurityCommandInterpreter(Plugin* const plugin);
  ~SecurityCommandInterpreter();
  int executeCommand(const std::vector<std::string> &command, CommandOutputWriter *outputWriter);

 private:
  int listCommands(CommandOutputWriter *outputWriter);
  int arm(const std::string &command, const std::vector<std::string> &arguments, CommandOutputWriter *outputWriter);
  int disarm(const std::string &command, const std::vector<std::string> &arguments, CommandOutputWriter *outputWriter);
  std::map<std::string, CommandFunction> commandFunctions_;
};

#endif // SECURITYCOMMANDINTERPRETER_H_
