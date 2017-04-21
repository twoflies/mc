// Shell Window

#include "shellwindow.h"

#include <algorithm>

#include "../util.h"

#define MAX_BUFFER_COUNT 50

ShellWindow::ShellWindow(const std::vector<Plugin*>& plugins, int height, int width, int y, int x) : Window(height, width, y, x), plugins_(plugins) {
  executing_ = false;
}

ShellWindow::~ShellWindow() {
}

int ShellWindow::mainLoop() {
  WINDOW* win = getWin();
  if (win == NULL) {
    return -1;
  }

  int height, width;
  if (getmaxyx(win, height, width) == ERR) {
    return -1;
  }
  width++;  // -Wall
  
  bool quit = false;
  while (!quit) {
    int c = wgetch(win);
    switch (c) {
    case 10:
      executeCommand(&quit);
      break;
    default:
      commandBuffer_.push_back(c);
      drawCommandLine(win, height - 1);
    }

    update_panels();
    doupdate();
  }

  return 0;
}

int ShellWindow::drawContent() {  
  WINDOW* win = getWin();
  if (win == NULL) {
    return -1;
  }

  int height, width;
  if (getmaxyx(win, height, width) == ERR) {
    return -1;
  }
  width++;  // -Wall

  if (!executing_) {
    --height;
  }

  int size = static_cast<int>(outputBuffer_.size());
  int lines = std::min(size, height);
  int index = std::max(size - lines, 0);
  int y = height - lines;
  for (; index < size; ++index) {
    mvwprintw(win, y++, 0, outputBuffer_[index].c_str());
    wclrtoeol(win);
  }

  if (!executing_) {
    drawCommandLine(win, y);
  }

  update_panels();

  return 0;
}

PluginCommandInterpreter* const ShellWindow::getPluginCommandInterpreter(const std::string& prefix) {
  for (std::vector<Plugin*>::const_iterator it = plugins_.begin(); it != plugins_.end(); ++it) {
    Plugin* plugin = (*it);
    if (prefix.compare(plugin->getPrefix()) == 0) {
      return plugin->getCommandInterpreter();
    }
  }
  return NULL;
}

int ShellWindow::executeCommand(bool* quit) {
  executing_ = true;

  std::string command = commandBuffer_;
  outputBuffer_.push_back(">" + commandBuffer_);
  while (outputBuffer_.size() > MAX_BUFFER_COUNT) {
    outputBuffer_.pop_front();
  }
  commandBuffer_.clear();
  drawContent();

  std::vector<std::string> candidates = split(command, ' ');
  std::vector<std::string> components;
  std::string buffer;
  for (std::vector<std::string>::iterator it = candidates.begin(); it != candidates.end(); it++) {
    std::string current = *it;
    if (current[0] == '"' && current[current.length() - 1] == '"') {
      components.push_back(current.substr(1, current.length() - 2));
    }
    else if (current[0] == '"') {
      if (!buffer.empty()) {
	outputBuffer_.push_back("Mismatched quote.");
	components.clear();
	break;
      }

      buffer = current.substr(1, current.length() - 1);
    }
    else if (current[current.length() - 1] == '"') {
      if (buffer.empty()) {
	outputBuffer_.push_back("Mismatched quote.");
	components.clear();
	break;
      }

      buffer += " " + current.substr(0, current.length() - 1);
      components.push_back(buffer);
      buffer.clear();
    }
    else if (!buffer.empty()) {
      buffer += " " + current;
    }
    else {
      components.push_back(current);
    }
  }

  if (!components.empty()) {
    PluginCommandInterpreter* const pluginCommandInterpreter = getPluginCommandInterpreter(components[0]);
    if (pluginCommandInterpreter != NULL) {
      std::vector<std::string> pluginCommand;
      if (components.size() > 1) {
	pluginCommand.insert(pluginCommand.begin(), components.begin() + 1, components.end());
      }
      pluginCommandInterpreter->executeCommand(pluginCommand);
    }
    else if (!executeShellCommand(components, quit)) {
      outputBuffer_.push_back("Unknown command: " + components[0]);
    }
  }

  executing_ = false;

  drawContent();

  return 0;
}

bool ShellWindow::executeShellCommand(const std::vector<std::string>& command, bool* quit) {
  if (command[0].compare("quit") == 0) {
    *quit = true;
    return true;
  }

  return false;
}

void ShellWindow::writeLine(const std::string& line) {
  outputBuffer_.push_back(line);
  drawContent();
  doupdate();
}

int ShellWindow::drawCommandLine(WINDOW* win, int y) {
  int x = 0;
  mvwprintw(win, y, x, ">");
  ++x;

  mvwprintw(win, y, x, commandBuffer_.c_str());
  
  wclrtoeol(win);

  return OK;
}
