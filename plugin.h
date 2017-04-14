// Plugin

#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <string>

#include "outputwriter.h"

class PluginCommandInterpreter;
class PluginWindow;

class Plugin {
 public:
  Plugin();
  virtual ~Plugin();
  virtual std::string getPrefix() = 0;
  virtual int initialize(OutputWriter* outputWriter);
  virtual int destroy() = 0;
  virtual PluginCommandInterpreter* const getCommandInterpreter() = 0;
  virtual PluginWindow* createConsoleWindow(int height, int width, int y, int x) = 0;
  virtual void destroyConsoleWindow(PluginWindow *window) = 0;

 protected:
  OutputWriter* outputWriter_;
};

#endif // PLUGIN_H_
