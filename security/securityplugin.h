#ifndef SECURITYPLUGIN_H_
#define SECURITYPLUGIN_H_

// Security Plugin

#include <string>
#include <vector>
#include <map>

#include "../plugin.h"
#include "../event.h"
#include "../outputwriter.h"
#include "../logger.h"
#include "xbmanager/manager.h"
#include "xbserial/iosample.h"
#include "securitycommandinterpreter.h"

const int ERR_INVALID_PASSCODE = -1000; 
const int ERR_SECURITY_ARMED = -1001;
const int ERR_SECURITY_DISARMED = -1002;
const int ERR_INVALID_NODE = -1003;

enum NodeStatus {
  NODE_STATUS_UNKNOWN,
  NODE_STATUS_OK,
  NODE_STATUS_FAULTED
};

struct Node {
  XB::Module* module;
  NodeStatus status;
  unsigned short voltage;
};

enum SecurityStatus {
  SECURITY_STATUS_UNKNOWN,
  SECURITY_STATUS_DISARMED,
  SECURITY_STATUS_ARMED,
  SECURITY_STATUS_FAULTED
};

const XB::PIN SENSOR_PIN = XB::D0;
const std::string DEFAULT_PASSCODE = "000";

struct SecurityData {
  std::string passcode;
  std::map<XB::Address64, std::string> identifierMap;

  SecurityData() {
    passcode = DEFAULT_PASSCODE;
  }
};

class SecurityPlugin : public Plugin, public XB::IOSampleFrameSubscriber {
 public:
  SecurityPlugin();
  ~SecurityPlugin();
  std::string getPrefix();
  int initialize(OutputWriter* outputWriter);
  int destroy();
  PluginCommandInterpreter* const getCommandInterpreter();
  PluginWindow* createConsoleWindow(int height, int width, int y, int x);
  void destroyConsoleWindow(PluginWindow *window);
  SecurityStatus getStatus();
  const std::vector<Node*>& getNodes();
  int discover();
  int arm(const std::string& passcode);
  int disarm(const std::string& passcode);
  int passcode(const std::string& oldPasscode, const std::string& passcode);
  Event<SecurityStatus>* const getStatusChangedEvent();
  int name(const std::string& oldName, const std::string& name);
  void received(const XB::IOSampleFrame* frame);

 private:
  int handleIOSample(Node* node, const XB::IOSampleFrame* frame);
  void setStatus(SecurityStatus status);
  SecurityData* readSecurityData(const std::string& path);
  int writeSecurityData(const std::string& path, SecurityData* data);
  XB::ModuleConfiguration* readModuleConfiguration(const std::string& path);
  Node* findNodeByIdentifier(const std::string& identifier);
  Node* findNodeByAddress(XB::Address16 address);
  int destroyNodes();
  
 private:
  Logger logger_;
  SecurityData* securityData_;
  XB::Manager manager_;
  SecurityCommandInterpreter* commandInterpreter_;
  SecurityStatus status_;
  std::vector<Node*> nodes_;
  std::map<unsigned short, Node*> nodeMap_;
  Event<SecurityStatus> statusChangedEvent_;
};

#endif // SECURITYPLUGIN_H_
