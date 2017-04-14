// Plugin Window

#ifndef PLUGINWINDOW_H_
#define PLUGINWINDOW_H_

#include <panel.h>

#include "window.h"
#include "../plugin.h"
#include "../event.h"

class PluginWindow : public Window {
 public:
  PluginWindow(Plugin* const plugin, int height, int width, int y, int x);
  int draw();
  Plugin* const getPlugin();
  Event<void>* const getDataChangedEvent();

 protected:
  void fireDataChangedEvent();

 private:
  Plugin* const plugin_;
  Event<void> dataChangedEvent_;
};

#endif // PLUGINWINDOW_H_
