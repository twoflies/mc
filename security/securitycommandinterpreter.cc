// Security Command Interpreter

#include "securitycommandinterpreter.h"

#include "securityplugin.h"

SecurityCommandInterpreter::SecurityCommandInterpreter(Plugin* const plugin) : PluginCommandInterpreter(plugin) {
  commandFunctions_["arm"] = &SecurityCommandInterpreter::arm;
  commandFunctions_["disarm"] = &SecurityCommandInterpreter::disarm;
}

SecurityCommandInterpreter::~SecurityCommandInterpreter() {
}

int SecurityCommandInterpreter::executeCommand(const std::vector<std::string> &command, CommandOutputWriter *outputWriter) {

  if (command.size() == 0) {
    return listCommands(outputWriter);
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
      return (this->*commandFunction)(commandKey, arguments, outputWriter);
    }
    else {
      outputWriter->writeLine(getPlugin()->getPrefix() + ": Unknown command: " + commandKey);
      return ERR;
    }
  }
}

int SecurityCommandInterpreter::listCommands(CommandOutputWriter *outputWriter) {
  for (std::map<std::string, CommandFunction>::iterator it = commandFunctions_.begin(); it != commandFunctions_.end(); ++it) {
    outputWriter->writeLine((*it).first);
  }
  
  return OK;
}

int SecurityCommandInterpreter::arm(const std::string &command, const std::vector<std::string> &arguments, CommandOutputWriter *outputWriter) {
  SecurityPlugin *plugin = static_cast<SecurityPlugin*>(getPlugin());

  if (arguments.size() > 0) {
    int result = plugin->arm(arguments[0]);
    if (result == ERR) {
      std::ostringstream stream;
      stream << command << " failed with " << result;
      outputWriter->writeLine(stream.str());
    }
    return result;
  }
  else {
    outputWriter->writeLine("Usage: " + command + " [PASSCODE]");
    return ERR;
  }
}

int SecurityCommandInterpreter::disarm(const std::string &command, const std::vector<std::string> &arguments, CommandOutputWriter *outputWriter) {
  SecurityPlugin *plugin = static_cast<SecurityPlugin*>(getPlugin());

  if (arguments.size() > 0) {
    int result = plugin->disarm(arguments[0]);
    if (result == ERR) {
      std::ostringstream stream;
      stream << command << " failed with " << result;
      outputWriter->writeLine(stream.str());
    }
    return result;
  }
  else {
    outputWriter->writeLine("Usage: " + command + " [PASSCODE]");
    return ERR;
  }
}


