// Security Plugin

#include "securitypluginwindow.h"

#define PREFIX "security";

SecurityPlugin::SecurityPlugin() {
  prefix_ = PREFIX;
  commandInterpreter_ = new SecurityCommandInterpreter(this); 
  status_ = SECURITY_STATUS_DISARMED;
  
  SensorNode *node = new SensorNode(0x01, "Back Door");
  nodes_.push_back(node);
  sensorNodes_.push_back(node);

  node = new SensorNode(0x02, "Main Door");
  nodes_.push_back(node);
  sensorNodes_.push_back(node);

  node = new SensorNode(0x03, "Front Door");
  nodes_.push_back(node);
  sensorNodes_.push_back(node);
}

SecurityPlugin::~SecurityPlugin() {
  delete commandInterpreter_;
}

const std::string &SecurityPlugin::getPrefix() const {
  return prefix_;
}

PluginCommandInterpreter *SecurityPlugin::getCommandInterpreter() {
  return commandInterpreter_;
}

PluginWindow *SecurityPlugin::createConsoleWindow(int height, int width, int y, int x) {
  return new SecurityPluginWindow(this, height, width, y, x);
}

void SecurityPlugin::destroyConsoleWindow(PluginWindow *window) {
  delete window;
}

const SecurityStatus SecurityPlugin::getStatus() const {
  return status_;
}

const std::vector<SensorNode*>& SecurityPlugin::getSensorNodes() const {
  return sensorNodes_;
}

int SecurityPlugin::arm(const std::string &passcode) {
  if (status_ == SECURITY_STATUS_ARMED) {
    return ERR_SECURITY_ARMED;
  }
  
  status_ = SECURITY_STATUS_ARMED;
  statusChangedEvent_.fire(&status_);
  return OK;
}

int SecurityPlugin::disarm(const std::string &passcode) {
  if (status_ == SECURITY_STATUS_DISARMED) {
    return ERR_SECURITY_DISARMED;
  }
  
  status_ = SECURITY_STATUS_DISARMED;
  statusChangedEvent_.fire(&status_);
  return OK;
}

Event<SecurityStatus> &SecurityPlugin::getStatusChangedEvent() {
  return statusChangedEvent_;
}
