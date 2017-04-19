// Logger

#ifndef _LOGGER__H_
#define _LOGGER_H_

#include <string>
#include <fstream>
#include <cstdarg>

class Logger {
  enum Level {
    INFO,
    WARN,
    ERROR
  };
  
 public:
  Logger();
  ~Logger();
  void open(const char* path);
  void close();
  void log(Level level, std::string message);
  void log(Level level, const char* format, va_list args);
  void log(Level level, const char* format, ...);
  void logInfo(std::string message) { log(INFO, message); }
  void logInfo(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(INFO, format, args);
    va_end(args);
  }
  void logWarn(std::string message) { log(WARN, message); }
  void logWarn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(WARN, format, args);
    va_end(args);
  }
  void logError(std::string message) { log(ERROR, message); }
  void logError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(ERROR, format, args);
    va_end(args);
  }
  
 private:
  std::ofstream stream_;
};

#endif // _LOGGER_H_
