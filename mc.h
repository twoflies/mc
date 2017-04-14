// Master Control

#ifndef MASTERCONTROL_H_
#define MASTERCONTROL_H_

#include <vector>

#include "plugin.h"

class MasterControl {
 public:
  MasterControl();
  ~MasterControl();
  int initialize();
  int destroy();
  const std::vector<Plugin*>& getPlugins();

 private:
  std::vector<Plugin*> plugins_;
};

#endif // MASTERCONTROL_H_
