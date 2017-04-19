// Security Plugin

#include "securitypluginwindow.h"

#include <fstream>

const std::string PREFIX("security");
const std::string DEFAULT_CONFIGURATION_PATH("conf/module");

SecurityPlugin::SecurityPlugin() {
  status_ = SECURITY_STATUS_UNKNOWN;
}

SecurityPlugin::~SecurityPlugin() {
}

int SecurityPlugin::initialize(OutputWriter* outputWriter) {
  int result = Plugin::initialize(outputWriter);
  if (result != 0) {
    return result;
  }

  commandInterpreter_ = new SecurityCommandInterpreter(this, outputWriter);

  logger_.open("mc.log");
  logger_.logInfo("============");
  logger_.logInfo("System start.");

  result = manager_.initialize();
  if (result != 0) {
    return result;
  }

  return 0;
}

int SecurityPlugin::destroy() {
  delete commandInterpreter_;
    
  int result = destroyNodes();
  if (result != 0) {
    return result;
  }

  /*result = manager_.destroy();
  if (result != 0) {
    return result;
    }*/

  logger_.logInfo("System stop.");
  logger_.close();

  return 0;
}

std::string SecurityPlugin::getPrefix() {
  return PREFIX;
}

PluginCommandInterpreter* const SecurityPlugin::getCommandInterpreter() {
  return commandInterpreter_;
}

PluginWindow* SecurityPlugin::createConsoleWindow(int height, int width, int y, int x) {
  return new SecurityPluginWindow(this, height, width, y, x);
}

void SecurityPlugin::destroyConsoleWindow(PluginWindow *window) {
  delete window;
}

SecurityStatus SecurityPlugin::getStatus() {
  return status_;
}

const std::vector<Node*>& SecurityPlugin::getNodes() {
  return nodes_;
}

int SecurityPlugin::discover() {
  int result = manager_.unsubscribeIOSample(this);
  if (result != 0) {
    return result;
  }

  destroyNodes();
  
  XB::ModuleConfiguration* configuration = readConfiguration(DEFAULT_CONFIGURATION_PATH);
  if (configuration == NULL) {
    return -1;
  }
  
  outputWriter_->writeLine("Discovering...");
  std::vector<XB::Module*> modules;
  result = manager_.discoverModules(modules);
  if (result != 0) {
    delete configuration;
    return result;
  }
  outputWriter_->writeLine("Discovered %d nodes.", modules.size());

  for (std::vector<XB::Module*>::iterator it = modules.begin(); it != modules.end(); it++) {
    XB::Module* module = (*it);

    if (module->identifier.find_first_not_of(' ') == std::string::npos) {
      outputWriter_->writeLine("Configuring node...");
      result = manager_.configureModule(module, configuration);
      if (result != 0) {
	outputWriter_->writeLine("Failed to configure node: %d.", result);
	logger_.logError("Failed to configure node: %d.", result);
      }
      else {
	outputWriter_->writeLine("Configured new node '%s'.", module->identifier.c_str());
	logger_.logInfo("Configured new node '%s'.", module->identifier.c_str());
      }
    }
    else {
      logger_.logInfo("Discovered existing node '%s'.", module->identifier.c_str());
    }

    Node* node = new Node();
    node->module = module;
    node->status = NODE_STATUS_UNKNOWN;
    nodes_.push_back(node);
    nodeMap_[module->address16.ushort()] = node;
  }

  delete configuration;
  setStatus(SECURITY_STATUS_DISARMED);

  return manager_.subscribeIOSample(this);
}

int SecurityPlugin::arm(const std::string& passcode) {
  if (status_ == SECURITY_STATUS_ARMED) {
    return ERR_SECURITY_ARMED;
  }
  
  setStatus(SECURITY_STATUS_ARMED);

  logger_.logInfo("System armed.");
  
  return 0;
}

int SecurityPlugin::disarm(const std::string& passcode) {
  if (status_ == SECURITY_STATUS_DISARMED) {
    return ERR_SECURITY_DISARMED;
  }

  setStatus(SECURITY_STATUS_DISARMED);

  logger_.logInfo("System disarmed.");
  
  return 0;
}

Event<SecurityStatus>* const SecurityPlugin::getStatusChangedEvent() {
  return &statusChangedEvent_;
}

void SecurityPlugin::received(const XB::IOSampleFrame* frame) {
  std::map<unsigned short, Node*>::iterator it = nodeMap_.find(frame->getAddress16().ushort());
  if (it != nodeMap_.end()) {
    handleIOSample((*it).second, frame);
  }
  else {
    logger_.logWarn("Received IO sample for unknown node: %02X-%02X", frame->getAddress16().a, frame->getAddress16().b);
  }
}

int SecurityPlugin::handleIOSample(Node* node, const XB::IOSampleFrame* frame) {
  SecurityStatus status = status_;
  
  if ((node->status != NODE_STATUS_FAULTED) || (status == SECURITY_STATUS_DISARMED)) {
    if (frame->isDigitalPinSet(SENSOR_PIN)) {
      node->status = NODE_STATUS_FAULTED;
      logger_.logInfo("Node '%s' Faulted.", node->module->identifier.c_str());
      if (status == SECURITY_STATUS_ARMED) {
	status = SECURITY_STATUS_FAULTED;
	logger_.logInfo("System Faulted.");
      }
    }
    else {
      bool log = (node->status != NODE_STATUS_OK);
      node->status = NODE_STATUS_OK;
      if (log) {
	logger_.logInfo("Node '%s' OK.", node->module->identifier.c_str());
      }
    }
  }
  node->voltage = frame->getVoltage();

  setStatus(status);

  return 0;
}

void SecurityPlugin::setStatus(SecurityStatus status) {
  status_ = status;
  statusChangedEvent_.fire(&status_);
}

XB::ModuleConfiguration* SecurityPlugin::readConfiguration(const std::string& path) {
  XB::ModuleConfiguration* configuration = new XB::ModuleConfiguration();
  
  std::ifstream infile(path.c_str());
  std::string command;
  unsigned short parameter;
  while (infile >> command >> std::hex >> parameter) {
    configuration->addCommandParameter(command.c_str(), parameter);
  }

  return configuration;
}

int SecurityPlugin::destroyNodes() {
  for (std::vector<Node*>::iterator it = nodes_.begin(); it != nodes_.end(); it++) {
    delete (*it);
  }
  nodes_.clear();
  nodeMap_.clear();

  return 0;
}
