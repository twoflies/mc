#ifndef SECURITYPLUGIN_H_
#define SECURITYPLUGIN_H_

// Security Plugin

#include <string>
#include <vector>

#include "../plugin.h"
#include "../event.h"
#include "securitycommandinterpreter.h"
#include "sensornode.h"

#define ERR_SECURITY_ARMED -1001
#define ERR_SECURITY_DISARMED -1002

enum SecurityStatus {
  SECURITY_STATUS_UNKNOWN,
  SECURITY_STATUS_DISARMED,
  SECURITY_STATUS_ARMED,
  SECURITY_STATUS_FAULTED
};

class SecurityPlugin : public Plugin {
 public:
  SecurityPlugin();
  ~SecurityPlugin();
  const std::string &getPrefix() const;
  PluginCommandInterpreter *getCommandInterpreter();
  PluginWindow *createConsoleWindow(int height, int width, int y, int x);
  void destroyConsoleWindow(PluginWindow *window);
  const SecurityStatus getStatus() const;
  const std::vector<SensorNode*>& getSensorNodes() const;
  int arm(const std::string &passcode);
  int disarm(const std::string &passcode);
  Event<SecurityStatus>& getStatusChangedEvent();
  
 private:
  std::string prefix_;
  SecurityCommandInterpreter *commandInterpreter_;
  SecurityStatus status_;
  std::vector<Node*> nodes_;
  std::vector<SensorNode*> sensorNodes_;
  Event<SecurityStatus> statusChangedEvent_;
};

#endif // SECURITYPLUGIN_H_
