#ifndef SECURITYPLUGIN_H_
#define SECURITYPLUGIN_H_

// Security Plugin

#include <string>
#include <vector>

#include "../plugin.h"
#include "../event.h"
#include "../outputwriter.h"
#include "../logger.h"
#include "xbmanager/manager.h"
#include "xbserial/iosample.h"
#include "securitycommandinterpreter.h"

#define ERR_SECURITY_ARMED -1001
#define ERR_SECURITY_DISARMED -1002

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
  Event<SecurityStatus>* const getStatusChangedEvent();
  void received(const XB::IOSampleFrame* frame);

 private:
  int handleIOSample(Node* node, const XB::IOSampleFrame* frame);
  void setStatus(SecurityStatus status);
  XB::ModuleConfiguration* readConfiguration(const std::string& path);
  int destroyNodes();
  
 private:
  Logger logger_;
  XB::Manager manager_;
  SecurityCommandInterpreter* commandInterpreter_;
  SecurityStatus status_;
  std::vector<Node*> nodes_;
  std::map<unsigned short, Node*> nodeMap_;
  Event<SecurityStatus> statusChangedEvent_;
};

#endif // SECURITYPLUGIN_H_
