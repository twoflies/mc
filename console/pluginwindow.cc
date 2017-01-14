// Plugin Window

#include "pluginwindow.h"

PluginWindow::PluginWindow(Plugin *plugin, int height, int width, int y, int x) : Window(height, width, y, x) {
  plugin_ = plugin;
}

int PluginWindow::draw() {
  WINDOW *win = getWin();
  if (win == NULL) {
    return ERR;
  }
  
  if (box(win, 0, 0) == ERR) {
    return ERR;
  }

  return Window::draw();
}

Plugin *PluginWindow::getPlugin() {
  return plugin_;
}

Event<void> &PluginWindow::getDataChangedEvent() {
  return dataChangedEvent_;
}

void PluginWindow::fireDataChangedEvent() {
  dataChangedEvent_.fire(NULL);
}
