// Window

#ifndef WINDOW_H_
#define WINDOW_H_

#include <panel.h>

class Window {
 public:
  Window(int height, int width, int y, int x);
  virtual ~Window();
  virtual int draw();
  virtual int drawContent() { return 0; };

 protected:
  WINDOW* const getWin();
  
 private:
  PANEL* panel_;
};

#endif // WINDOW_H_
