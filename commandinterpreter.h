// Command Interpreter

#ifndef COMMANDINTERPRETER_H_
#define COMMANDINTERPRETER_H_

#include <string>
#include <vector>

#include "outputwriter.h"

class CommandInterpreter {
 public:
  CommandInterpreter(OutputWriter* const outputWriter);
  virtual ~CommandInterpreter();
  virtual int executeCommand(const std::vector<std::string>& command) = 0;

 protected:
  OutputWriter* const outputWriter_;
};

#endif // COMMANDINTERPRETER_H_
