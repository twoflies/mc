// Window

#include "window.h"

Window::Window(int height, int width, int y, int x) {
  WINDOW *win = newwin(height, width, y, x);
  panel_ = new_panel(win);
}

Window::~Window() {
  WINDOW *win = panel_window(panel_);
  
  del_panel(panel_);
  
  delwin(win);
}

int Window::draw() {
  return drawContent();
}

WINDOW *Window::getWin() {
  return panel_window(panel_);
}
