// Plugin Window

#include "pluginwindow.h"

PluginWindow::PluginWindow(Plugin* const plugin, int height, int width, int y, int x) : Window(height, width, y, x), plugin_(plugin) {
}

int PluginWindow::draw() {
  WINDOW *win = getWin();
  if (win == NULL) {
    return -1;
  }
  
  if (box(win, 0, 0) != 0) {
    return -1;
  }

  return Window::draw();
}

Plugin* const PluginWindow::getPlugin() {
  return plugin_;
}

Event<void>* const PluginWindow::getDataChangedEvent() {
  return &dataChangedEvent_;
}

void PluginWindow::fireDataChangedEvent() {
  dataChangedEvent_.fire(NULL);
}
