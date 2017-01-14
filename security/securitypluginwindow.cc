// Security Plugin Window

#include "securitypluginwindow.h"

SecurityPluginWindow::SecurityPluginWindow(Plugin *plugin, int height, int width, int y, int x) : PluginWindow(plugin, height, width, y, x) {
  securityPlugin_ = dynamic_cast<SecurityPlugin*>(plugin);

  securityStatusChangedHandler_ = new EventHandler<SecurityPluginWindow, SecurityStatus>(this, &SecurityPluginWindow::securityStatusChanged);
  
  securityPlugin_->getStatusChangedEvent().addHandler(securityStatusChangedHandler_);
  
  infoColor_ = ConsoleColors::registerColorPair(COLOR_GREEN, COLOR_BLACK);
  warningColor_ = ConsoleColors::registerColorPair(COLOR_RED, COLOR_WHITE);
  errorColor_ = ConsoleColors::registerColorPair(COLOR_WHITE, COLOR_RED);
}

SecurityPluginWindow::~SecurityPluginWindow() {
  securityPlugin_->getStatusChangedEvent().removeHandler(securityStatusChangedHandler_);

  delete securityStatusChangedHandler_;
}

int SecurityPluginWindow::drawContent() { 
  WINDOW *win = getWin();
  if (win == NULL) {
    return ERR;
  }

  int height, width;
  if (getmaxyx(win, height, width) == ERR) {
    return ERR;
  }

  int y = 1, x = 2;
  
  std::string statusString = translate("Status: ");
  mvwprintw(win, y, x, statusString.c_str());
  x += statusString.length();

  statusString = getString(securityPlugin_->getStatus());
  int attributes = getAttributes(securityPlugin_->getStatus());
  wattron(win, attributes);
  mvwprintw(win, y, x, translate(statusString).c_str());
  wattroff(win, attributes);
  wclrtoeol(win);
  ++y;
  
  mvwhline(win, y, 1, ACS_HLINE, width - 2);
  ++y;
  x = 2;

  std::vector<SensorNode*> sensorNodes = securityPlugin_->getSensorNodes();
  for(std::vector<SensorNode*>::iterator it = sensorNodes.begin(); it != sensorNodes.end(); ++it) {
    SensorNode *node = (*it);

    std::ostringstream s;
    s << translate("Sensor ") << std::hex << std::showbase << node->getAddress() << std::dec << " (" << node->getLabel() <<"): ";
    statusString = s.str();
    mvwprintw(win, y, x, statusString.c_str());
    x += std::max(statusString.length(), 25ul);
    
    statusString = getString((*it)->getState());
    attributes = getAttributes((*it)->getState());
    wattron(win, attributes);
    mvwprintw(win, y, x, translate(statusString).c_str());
    wattroff(win, attributes);
    wclrtoeol(win);
    ++y;
    x = 2;

    if (y == (height - 1)) {
      break;
    }
  }

  refresh();

  return OK;
}

std::string SecurityPluginWindow::getString(SecurityStatus status) {
  switch (status) {
  case SECURITY_STATUS_DISARMED:
    return translate("Disarmed");
  case SECURITY_STATUS_ARMED:
    return translate("Armed");
  case SECURITY_STATUS_FAULTED:
    return translate("Faulted");
  default:
    return translate("Unknown");
  }
}

int SecurityPluginWindow::getAttributes(SecurityStatus status) {
  switch (status) {
  case SECURITY_STATUS_DISARMED:
    return getInfoAttributes();
  case SECURITY_STATUS_ARMED:
    return getWarningAttributes();
  case SECURITY_STATUS_FAULTED:
    return getErrorAttributes();
  default:
    return 0;
  }
}

std::string SecurityPluginWindow::getString(SensorState state) {
  switch (state) {
  case SENSOR_STATE_OK:
    return translate("OK");
  case SENSOR_STATE_FAULTED:
    return translate("Faulted");
  default:
    return translate("Unknown");
  }
}

int SecurityPluginWindow::getAttributes(SensorState state) {
  switch (state) {
  case SENSOR_STATE_OK:
    return getInfoAttributes();
  case SENSOR_STATE_FAULTED:
    return getErrorAttributes();
  default:
    return 0;
  }
}

int SecurityPluginWindow::getInfoAttributes() {
  return COLOR_PAIR(infoColor_);
}

int SecurityPluginWindow::getWarningAttributes() {
  return COLOR_PAIR(warningColor_);
}

int SecurityPluginWindow::getErrorAttributes() {
  return COLOR_PAIR(errorColor_) | A_BLINK;
}

void SecurityPluginWindow::securityStatusChanged(const SecurityStatus *status) {
  fireDataChangedEvent();
}
