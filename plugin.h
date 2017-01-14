
#ifndef PLUGIN_H_
#define PLUGIN_H_

// Plugin

#include <string>

class PluginCommandInterpreter;
class PluginWindow;

class Plugin {
 public:
  Plugin();
  virtual ~Plugin();
  virtual const std::string &getPrefix() const = 0;
  virtual PluginCommandInterpreter *getCommandInterpreter() = 0;
  virtual PluginWindow *createConsoleWindow(int height, int width, int y, int x) = 0;
  virtual void destroyConsoleWindow(PluginWindow *window) = 0;
};

#endif // PLUGIN_H_
