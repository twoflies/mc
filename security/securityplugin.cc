// Security Plugin

#include "securitypluginwindow.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "../util.h"

const std::string PREFIX("security");
const std::string DEFAULT_CONFIGURATION_PATH("conf/module.conf");
const std::string DEFAULT_LOG_PATH("log/mc.log");
const std::string DEFAULT_DATA_PATH("data/mc.data");

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

  logger_.open(DEFAULT_LOG_PATH.c_str());
  logger_.logInfo("============");
  logger_.logInfo("System start.");

  securityData_ = readSecurityData(DEFAULT_DATA_PATH);
  if (securityData_ == NULL) {
    return -1;
  }

  /*outputWriter->writeLine(securityData_->passcode);
  for (std::map<XB::Address64,std::string>::iterator it = securityData_->identifierMap.begin(); it != securityData_->identifierMap.end(); it++) {
    byte buffer[8];
    it->first.data(buffer);
    
    std::ostringstream address;
    writeHexString(address, buffer, sizeof(buffer));
    outputWriter->writeLine("%s -> %s", address.str().c_str(), it->second.c_str());
    }*/

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
  
  XB::ModuleConfiguration* configuration = readModuleConfiguration(DEFAULT_CONFIGURATION_PATH);
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
  outputWriter_->writeLine("Discovered %d node(s).", modules.size());

  for (std::vector<XB::Module*>::iterator it = modules.begin(); it != modules.end(); it++) {
    XB::Module* module = (*it);

    std::string existingIdentifier;
    std::map<XB::Address64,std::string>::iterator mit = securityData_->identifierMap.find(module->address64);
    if (mit != securityData_->identifierMap.end()) {
      existingIdentifier = mit->second;
    }

    if (module->identifier.find_first_not_of(' ') == std::string::npos) {
      configuration->identifier = existingIdentifier;
      result = manager_.configureModule(module, configuration);
      if (result != 0) {
	outputWriter_->writeLine("Failed to configure node: %d.", result);
	logger_.logError("Failed to configure node: %d.", result);
      }
      else {
	outputWriter_->writeLine("Configured node '%s'.", module->identifier.c_str());
	logger_.logInfo("Configured node '%s'.", module->identifier.c_str());
      }
    }
    else {
      if (!existingIdentifier.empty()) {
	module->identifier = existingIdentifier;
      }
      logger_.logInfo("Node '%s' already configured.", module->identifier.c_str());
      outputWriter_->writeLine("Node '%s' already configured.", module->identifier.c_str());
    }

    Node* node = new Node();
    node->module = module;
    node->status = NODE_STATUS_UNKNOWN;
    nodes_.push_back(node);
    nodeMap_[module->address16.ushort()] = node;

    securityData_->identifierMap[module->address64] = module->identifier;
  }

  delete configuration;
  setStatus(SECURITY_STATUS_DISARMED);

  result = writeSecurityData(DEFAULT_DATA_PATH, securityData_);
  if (result != 0) {
    return result;
  }

  return manager_.subscribeIOSample(this);
}

int SecurityPlugin::arm(const std::string& passcode) {
  if (securityData_->passcode.compare(passcode) != 0) {
    return ERR_INVALID_PASSCODE;
  }
    
  if (status_ == SECURITY_STATUS_ARMED) {
    return ERR_SECURITY_ARMED;
  }
  
  setStatus(SECURITY_STATUS_ARMED);

  logger_.logInfo("System armed.");
  
  return 0;
}

int SecurityPlugin::disarm(const std::string& passcode) {
  if (securityData_->passcode.compare(passcode) != 0) {
    return ERR_INVALID_PASSCODE;
  }

  if (status_ == SECURITY_STATUS_DISARMED) {
    return ERR_SECURITY_DISARMED;
  }

  setStatus(SECURITY_STATUS_DISARMED);

  logger_.logInfo("System disarmed.");
  
  return 0;
}

int SecurityPlugin::passcode(const std::string& oldPasscode, const std::string& passcode) {
  if (!securityData_->passcode.empty() && (securityData_->passcode.compare(oldPasscode) != 0)) {
    return ERR_INVALID_PASSCODE;
  }

  securityData_->passcode = passcode;

  return writeSecurityData(DEFAULT_DATA_PATH, securityData_);
}

int SecurityPlugin::name(const std::string& oldName, const std::string& name) {
  Node* node = findNodeByIdentifier(oldName);
  if (node == NULL) {
    return ERR_INVALID_NODE;
  }

  int result = manager_.setModuleIdentifier(node->module, name.c_str());
  if (result != 0) {
    return result;
  }

  node->module->identifier = name;
  securityData_->identifierMap[node->module->address64] = name;

  return writeSecurityData(DEFAULT_DATA_PATH, securityData_);
}

Event<SecurityStatus>* const SecurityPlugin::getStatusChangedEvent() {
  return &statusChangedEvent_;
}

void SecurityPlugin::received(const XB::IOSampleFrame* frame) {
  Node* node = findNodeByAddress(frame->getAddress16());
  if (node != NULL) {
    handleIOSample(node, frame);
  }
  else {
    logger_.logWarn("Received IO sample for unknown node: %02X-%02X", frame->getAddress16().a, frame->getAddress16().b);
  }
}

int SecurityPlugin::handleIOSample(Node* node, const XB::IOSampleFrame* frame) {
  SecurityStatus status = status_;
  
  if ((node->status != NODE_STATUS_FAULTED) || (status == SECURITY_STATUS_DISARMED)) {
    if (frame->isDigitalPinSet(SENSOR_PIN)) {
      bool log = (node->status != NODE_STATUS_FAULTED);
      node->status = NODE_STATUS_FAULTED;
      if (log) {
	logger_.logInfo("Node '%s' Faulted.", node->module->identifier.c_str());
      }

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

SecurityData* SecurityPlugin::readSecurityData(const std::string& path) {
  SecurityData* data = new SecurityData();

  std::ifstream infile(path.c_str());
  if (!infile.is_open()) {
    return data;
  }

  std::getline(infile, data->passcode);

  std::string address;
  byte buffer[8];
  while (std::getline(infile, address)) {
    std::string identifier;
    std::getline(infile, identifier);

    std::istringstream instring(address);
    readHexString(instring, buffer, sizeof(buffer));
    XB::Address64 address(buffer);

    data->identifierMap[address] = identifier;
  }

  return data;
}

int SecurityPlugin::writeSecurityData(const std::string& path, SecurityData* data) {
  std::ofstream outfile(path.c_str());
  if (!outfile.is_open()) {
    return -1;
  }

  outfile << data->passcode << std::endl;

  byte buffer[8];
  for (std::map<XB::Address64, std::string>::iterator it = data->identifierMap.begin(); it != data->identifierMap.end(); it++) {
    it->first.data(buffer);
    std::ostringstream outstring;
    writeHexString(outstring, buffer, sizeof(buffer));
    outfile << outstring.str() << std::endl;
    outfile << it->second << std::endl;
  }

  return 0;
}

XB::ModuleConfiguration* SecurityPlugin::readModuleConfiguration(const std::string& path) {
  XB::ModuleConfiguration* configuration = new XB::ModuleConfiguration();
  
  std::ifstream infile(path.c_str());
  std::string command;
  unsigned short parameter;
  while (infile >> command >> std::hex >> parameter) {
    configuration->addCommandParameter(command.c_str(), parameter);
  }

  return configuration;
}

Node* SecurityPlugin::findNodeByIdentifier(const std::string& identifier) {
  for (std::vector<Node*>::iterator it = nodes_.begin(); it != nodes_.end(); it++) {
    if ((*it)->module->identifier.compare(identifier) == 0) {
      return (*it);
    }
  }

  return NULL;
}

Node* SecurityPlugin::findNodeByAddress(XB::Address16 address) {
  std::map<unsigned short, Node*>::iterator it = nodeMap_.find(address.ushort());
  return (it != nodeMap_.end()) ? it->second : NULL;
}

int SecurityPlugin::destroyNodes() {
  for (std::vector<Node*>::iterator it = nodes_.begin(); it != nodes_.end(); it++) {
    delete (*it);
  }
  nodes_.clear();
  nodeMap_.clear();

  return 0;
}
