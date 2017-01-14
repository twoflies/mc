// Master Control

#include "mc.h"
#include "security/securityplugin.h"

MasterControl::MasterControl() {
  plugins_.push_back(new SecurityPlugin());
}

int MasterControl::getPluginCount() {
  return plugins_.size();
}

const std::vector<Plugin*> *MasterControl::getPlugins() {
  return &plugins_;
}
