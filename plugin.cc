// Plugin

#include "plugin.h"

Plugin::Plugin() {
}

Plugin::~Plugin() {
}

int Plugin::initialize(OutputWriter* outputWriter) {
  outputWriter_ = outputWriter;

  return 0;
}
