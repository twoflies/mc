#ifndef COMMANDINTERPRETER_H_
#define COMMANDINTERPRETER_H_

// Command Interpreter

#include <string>
#include <vector>

#ifndef ERR
#define ERR (-1)
#endif

#ifndef OK
#define OK (0)
#endif

class CommandOutputWriter {
 public:
  CommandOutputWriter() {};
  virtual ~CommandOutputWriter() {};
  virtual void writeLine(const std::string &line) = 0;
};

class CommandInterpreter {
 public:
  CommandInterpreter();
  virtual ~CommandInterpreter();
  virtual int executeCommand(const std::vector<std::string> &command, CommandOutputWriter *outputWriter) = 0;
};

#endif // COMMANDINTERPRETER_H_
