// Output Writer

#include "outputwriter.h"

#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>

OutputWriter::OutputWriter() {
}

OutputWriter::~OutputWriter() {
}

void OutputWriter::writeLine(const char* format, ...) {
  va_list args;
  va_start(args, format);
  char* message;
  int result = vasprintf(&message, format, args);
  if (result != -1) {
    writeLine(std::string(message));
    free(message);
  }
  va_end(args);
}
