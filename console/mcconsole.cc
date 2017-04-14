// Master Control Console (ncurses)

#include "mcconsole.h"

#include <panel.h>

MasterControlConsole::MasterControlConsole(MasterControl* const masterControl) : masterControl_(masterControl) {
}

MasterControlConsole::~MasterControlConsole() {
}

int MasterControlConsole::initialize() {
  initscr();
  //raw();
  cbreak();
  noecho();
  start_color();

  int MID = LINES / 2;

  pluginDataChangedEventHandler_ = new EventHandler<MasterControlConsole, void>(this, &MasterControlConsole::pluginDataChanged);
  
  const std::vector<Plugin*>& plugins = masterControl_->getPlugins();

  shellWindow_ = new ShellWindow(plugins, LINES - MID, COLS, MID, 0);
  
  for (std::vector<Plugin*>::const_iterator it = plugins.begin(); it != plugins.end(); it++) {
    Plugin* plugin = (*it);
    PluginWindow* pluginWindow = plugin->createConsoleWindow(MID, COLS, 0, 0);
    if (pluginWindow == NULL) {
      return -1;
    }
    pluginWindow->getDataChangedEvent()->addHandler(pluginDataChangedEventHandler_);
    pluginWindows_.push_back(pluginWindow);

    int result = plugin->initialize(shellWindow_);
    if (result != 0) {
      return -1;
    }
  }
  
  return 0;
}

int MasterControlConsole::destroy() {
  for (std::vector<PluginWindow*>::iterator it = pluginWindows_.begin(); it != pluginWindows_.end(); it++) {
    PluginWindow* window = *it;
    Plugin* const plugin = window->getPlugin();
    int result = plugin->destroy();
    if (result != 0) {
      return -1;
    }
    plugin->destroyConsoleWindow(window);
  }
  pluginWindows_.clear();
  
  delete shellWindow_;
  
  endwin();

  return 0;
}

int MasterControlConsole::mainLoop() {
  shellWindow_->draw();

  refreshPluginWindows();
    
  return shellWindow_->mainLoop();
}

int MasterControlConsole::refreshPluginWindows() {
  for (std::vector<PluginWindow*>::iterator it = pluginWindows_.begin(); it != pluginWindows_.end(); it++) {
    (*it)->draw();
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
