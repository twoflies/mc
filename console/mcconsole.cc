// Master Control Console (ncurses)

#include "mcconsole.h"

#include <panel.h>

#define CITERATOR(T,v,n) for(std::vector<T>::const_iterator (n) = (v).begin(); (n) != (v).end(); ++(n))

#define ITERATOR(T,v,n) for(std::vector<T>::iterator (n) = (v).begin(); (n) != (v).end(); ++(n))

MasterControlConsole::MasterControlConsole(MasterControl *masterControl) {
  masterControl_ = masterControl;
  
  initscr();
  //raw();
  cbreak();
  noecho();
  start_color();

  pluginDataChangedEventHandler_ = new EventHandler<MasterControlConsole, void>(this, &MasterControlConsole::pluginDataChanged);
  const std::vector<Plugin*> *plugins = masterControl->getPlugins();
  CITERATOR(Plugin*, *plugins, it) {
    PluginWindow *window = (*it)->createConsoleWindow(LINES / 2, COLS, 0, 0);
    window->getDataChangedEvent().addHandler(pluginDataChangedEventHandler_);
    pluginWindows_.push_back(window);
  }
  
  int y = LINES / 2;
  shellWindow_ = new ShellWindow(plugins, LINES - y, COLS, y, 0);
}

MasterControlConsole::~MasterControlConsole() {
  ITERATOR(PluginWindow*, pluginWindows_, it) {
    PluginWindow *window = *it;
    Plugin *plugin = window->getPlugin();
    plugin->destroyConsoleWindow(window);
  }
  pluginWindows_.clear();
  
  delete shellWindow_;
  
  endwin();
}

int MasterControlConsole::mainLoop() {
  shellWindow_->draw();

  refreshPluginWindows();
    
  return shellWindow_->mainLoop();
}

int MasterControlConsole::refreshPluginWindows() {
  ITERATOR(PluginWindow*, pluginWindows_, it) {
    PluginWindow *window = *it;
    window->draw();
  }

  update_panels();
  doupdate();

  return OK;
}

void MasterControlConsole::pluginDataChanged(const void *data) {
  refreshPluginWindows();
}

short ConsoleColors::colors_ = 0;

short ConsoleColors::registerColorPair(short foreground, short background) {
  for (int pair = 1; pair <= colors_; pair++) {
    short pairForeground, pairBackground;
    if (pair_content(pair, &pairForeground, &pairBackground) != ERR) {
      if ((pairForeground == foreground) && (pairBackground == background)) {
	return pair;
      }
    }
  }

  if (init_pair(++colors_, foreground, background) == ERR) {
    return ERR;
  }

  return colors_;
}
