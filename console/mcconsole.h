// Master Constrol Console (ncsurses)

#ifndef MCCONSOLE_H_
#define MCCONSOLE_H_

#include <vector>

#include "../mc.h"
#include "pluginwindow.h"
#include "shellwindow.h"

class MasterControlConsole {
 public:
  MasterControlConsole(MasterControl *masterControl);
  ~MasterControlConsole();
  int mainLoop();

 private:
  int refreshPluginWindows();
  void pluginDataChanged(const void*);
  MasterControl *masterControl_;
  std::vector<PluginWindow*> pluginWindows_;
  ShellWindow *shellWindow_;
  EventHandler<MasterControlConsole,void> *pluginDataChangedEventHandler_;
};

class ConsoleColors {
 public:
  static short registerColorPair(short foreground, short background);
  
 private:
  ConsoleColors() {};
  static short colors_;
};

#endif // MCCONSOLE_H_
