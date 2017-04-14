// Master Control

#include "mc.h"
#include "security/securityplugin.h"

MasterControl::MasterControl() {
}

MasterControl::~MasterControl() {
}

int MasterControl::initialize() {
  plugins_.push_back(new SecurityPlugin());

  return 0;
}

int MasterControl::destroy() {
  while (!plugins_.empty()) {
    delete plugins_.back();
    plugins_.pop_back();
  }

  return 0;
}

const std::vector<Plugin*>& MasterControl::getPlugins() {
  return plugins_;
}
