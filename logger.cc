// Logger

#include "logger.h"

#include <sstream>
#include <time.h>
#include <stdlib.h>

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::open(const char* path) {
  stream_.open(path, std::ofstream::app);
}

void Logger::close() {
  stream_.close();
}

void Logger::log(Level level, std::string message) {
  std::ostringstream stream;
  switch (level) {
  case INFO:
    stream << "INFO: ";
    break;
  case WARN:
    stream << "WARN: ";
    break;
  case ERROR:
    stream << "ERROR: ";
    break;
  }

  time_t current;
  struct tm* local;

  if (time(&current) != -1) {
    if ((local = localtime(&current)) != NULL) {
      char buffer[80];
      if (strftime(buffer, sizeof(buffer), "[%d-%m-%Y %I:%M:%S] ", local) > 0) {
	stream << buffer;
      }
    }
  }

  stream << message;

  stream_ << stream.str() << std::endl;
}

void Logger::log(Level level, const char* format, va_list args) {
  char* message;
  int result = vasprintf(&message, format, args);
  if (result != -1) {
    log(level, std::string(message));
    free(message);
  }
}

void Logger::log(Level level, const char* format, ...) {
  va_list args;
  va_start(args, format);

  log(level, format, args);
  
  va_end(args);
}
