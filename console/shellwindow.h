#ifndef SHELLWINDOW_H_
#define SHELLWINDOW_H_

// Shell Window

#include <vector>
#include <deque>
#include <string>
#include <panel.h>

#include "window.h"
#include "../plugincommandinterpreter.h"

class ShellWindow : public Window, public CommandOutputWriter {
 public:
  ShellWindow(const std::vector<Plugin*> *plugins, int height, int width, int y, int x);
  ~ShellWindow();
  int mainLoop();
  int drawContent();

 private:
  PluginCommandInterpreter *getPluginCommandInterpreter(const std::string &prefix) const;
  int executeCommand(bool& quit);
  bool executeShellCommand(const std::vector<std::string> &command, bool &quit);
  void writeLine(const std::string &line);
  int drawCommandLine(WINDOW *win, int y);
  
  const std::vector<Plugin*> *plugins_;
  std::deque<std::string> outputBuffer_;
  std::string commandBuffer_;
  bool executing_;
};

#endif // SHELLWINDOW_H_
