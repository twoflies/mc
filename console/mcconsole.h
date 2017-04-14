// Master Constrol Console (ncsurses)

#ifndef MCCONSOLE_H_
#define MCCONSOLE_H_

#include <vector>

#include "../mc.h"
#include "pluginwindow.h"
#include "shellwindow.h"

class MasterControlConsole {
 public:
  MasterControlConsole(MasterControl* const masterControl);
  ~MasterControlConsole();
  int initialize();
  int destroy();
  int mainLoop();

 private:
  int refreshPluginWindows();
  void pluginDataChanged(const void*);

 private:
  MasterControl* const masterControl_;
  EventHandler<MasterControlConsole, void>* pluginDataChangedEventHandler_;
  ShellWindow* shellWindow_;
  std::vector<PluginWindow*> pluginWindows_;
};

class ConsoleColors {
 public:
  static short registerColorPair(short foreground, short background);
  
 private:
  ConsoleColors() {};
  static short colors_;
};

#endif // MCCONSOLE_H_
