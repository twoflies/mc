// Security Command Interpreter

#include "securitycommandinterpreter.h"

#include <sstream>
#include "securityplugin.h"

SecurityCommandInterpreter::SecurityCommandInterpreter(Plugin* const plugin, OutputWriter* const outputWriter) : PluginCommandInterpreter(plugin, outputWriter) {
  commandFunctions_["discover"] = &SecurityCommandInterpreter::discover;
  commandFunctions_["arm"] = &SecurityCommandInterpreter::arm;
  commandFunctions_["disarm"] = &SecurityCommandInterpreter::disarm;
}

SecurityCommandInterpreter::~SecurityCommandInterpreter() {
}

int SecurityCommandInterpreter::executeCommand(const std::vector<std::string>& command) {

  if (command.size() == 0) {
    return listCommands();
  }
  else {
    std::string commandKey = command[0];
    std::map<std::string, CommandFunction>::iterator it = commandFunctions_.find(commandKey);
    if (it != commandFunctions_.end()) {
      CommandFunction commandFunction = (*it).second;
      std::vector<std::string> arguments;
      if (command.size() > 1) {
	arguments.insert(arguments.begin(), command.begin() + 1, command.end());
      }
      return (this->*commandFunction)(commandKey, arguments);
    }
    else {
      outputWriter_->writeLine(getPlugin()->getPrefix() + ": Unknown command: " + commandKey);
      return -1;
    }
  }
}

int SecurityCommandInterpreter::listCommands() {
  for (std::map<std::string, CommandFunction>::iterator it = commandFunctions_.begin(); it != commandFunctions_.end(); ++it) {
    outputWriter_->writeLine((*it).first);
  }
  
  return 0;
}

int SecurityCommandInterpreter::discover(const std::string& command, const std::vector<std::string>& arguments) {
  SecurityPlugin *plugin = dynamic_cast<SecurityPlugin*>(getPlugin());
 
  int result = plugin->discover();
  if (result != 0) {
    std::ostringstream stream;
    stream << command << " failed with " << result;
    outputWriter_->writeLine(stream.str());
  }
  return result;
}

int SecurityCommandInterpreter::arm(const std::string& command, const std::vector<std::string>& arguments) {
  SecurityPlugin *plugin = dynamic_cast<SecurityPlugin*>(getPlugin());

  if (arguments.size() > 0) {
    int result = plugin->arm(arguments[0]);
    if (result != 0) {
      std::ostringstream stream;
      stream << command << " failed with " << result;
      outputWriter_->writeLine(stream.str());
    }
    return result;
  }
  else {
    outputWriter_->writeLine("Usage: " + command + " [PASSCODE]");
    return -1;
  }
}

int SecurityCommandInterpreter::disarm(const std::string& command, const std::vector<std::string>& arguments) {
  SecurityPlugin *plugin = dynamic_cast<SecurityPlugin*>(getPlugin());

  if (arguments.size() > 0) {
    int result = plugin->disarm(arguments[0]);
    if (result != 0) {
      std::ostringstream stream;
      stream << command << " failed with " << result;
      outputWriter_->writeLine(stream.str());
    }
    return result;
  }
  else {
    outputWriter_->writeLine("Usage: " + command + " [PASSCODE]");
    return -1;
  }
}


