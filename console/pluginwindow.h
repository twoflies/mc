// Plugin Window

#ifndef PLUGINWINDOW_H_
#define PLUGINWINDOW_H_

#include <panel.h>

#include "window.h"
#include "../plugin.h"
#include "../event.h"

class PluginWindow : public Window {
 public:
  PluginWindow(Plugin *plugin, int height, int width, int y, int x);
  int draw();
  Plugin *getPlugin();
  Event<void> &getDataChangedEvent();

 protected:
  void fireDataChangedEvent();

 private:
  Plugin* plugin_;
  Event<void> dataChangedEvent_;
};

#endif // PLUGINWINDOW_H_
