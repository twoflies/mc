#ifndef SHELLWINDOW_H_
#define SHELLWINDOW_H_

// Shell Window

#include <vector>
#include <deque>
#include <string>
#include <panel.h>

#include "window.h"
#include "../plugincommandinterpreter.h"
#include "../outputwriter.h"

class ShellWindow : public Window, public OutputWriter {
 public:
  ShellWindow(const std::vector<Plugin*>& plugins, int height, int width, int y, int x);
  ~ShellWindow();
  int mainLoop();
  int drawContent();

 private:
  PluginCommandInterpreter* const getPluginCommandInterpreter(const std::string& prefix);
  int executeCommand(bool* quit);
  bool executeShellCommand(const std::vector<std::string>& command, bool* quit);
  void writeLine(const std::string& line);
  int drawCommandLine(WINDOW* win, int y);

 private:
  const std::vector<Plugin*>& plugins_;
  std::deque<std::string> outputBuffer_;
  std::string commandBuffer_;
  bool executing_;
};

#endif // SHELLWINDOW_H_
