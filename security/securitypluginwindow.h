#ifndef SECURITYPLUGINWINDOW_H_
#define SECURITYPLUGINWINDOW_H_

// Security Plugin Window

#include <string>
#include <panel.h>

#include "../locale.h"
#include "securityplugin.h"
#include "../console/mcconsole.h"
#include "../console/pluginwindow.h"

class SecurityPluginWindow : public PluginWindow {
 public:
  SecurityPluginWindow(Plugin* plugin, int height, int width, int y, int x);
  ~SecurityPluginWindow();
  int drawContent();

 private:
  std::string getString(SecurityStatus status);
  int getAttributes(SecurityStatus status);
  std::string getString(NodeStatus status);
  int getAttributes(NodeStatus status);
  int getInfoAttributes();
  int getWarningAttributes();
  int getErrorAttributes();
  void securityStatusChanged(const SecurityStatus* status);

 private:
  SecurityPlugin* securityPlugin_;
  EventHandler<SecurityPluginWindow, SecurityStatus>* securityStatusChangedHandler_;
  short infoColor_;
  short warningColor_;
  short errorColor_;
};

#endif // SECURITYPLUGINWINDOW_H_
