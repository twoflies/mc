#ifndef SECURITYCOMMANDINTERPRETER_H_
#define SECURITYCOMMANDINTERPRETER_H_

// Security Command Interpreter

#include <string>
#include <map>

#include "../plugincommandinterpreter.h"

class SecurityCommandInterpreter : public PluginCommandInterpreter {
  typedef int (SecurityCommandInterpreter::*CommandFunction)(const std::string&, const std::vector<std::string>&);
  
 public:
  SecurityCommandInterpreter(Plugin* const plugin, OutputWriter* const outputWriter);
  ~SecurityCommandInterpreter();
  int executeCommand(const std::vector<std::string>& command);

 private:
  int listCommands();
  int discover(const std::string& command, const std::vector<std::string>& arguments);
  int arm(const std::string& command, const std::vector<std::string>& arguments);
  int disarm(const std::string& command, const std::vector<std::string>& arguments);
  int passcode(const std::string& command, const std::vector<std::string>& arguments);
  int name(const std::string& command, const std::vector<std::string>& arguments);

 private:
  std::map<std::string, CommandFunction> commandFunctions_;
};

#endif // SECURITYCOMMANDINTERPRETER_H_
