#ifndef SENSORNODE_H_
#define SENSORNODE_H_

// Sensor Node

#include <string>
#include <sstream>

#include "node.h"

enum SensorState {
  SENSOR_STATE_UNKNOWN,
  SENSOR_STATE_OK,
  SENSOR_STATE_FAULTED
};

class SensorNode : public Node {
 public:
  SensorNode(int address, std::string label);
  ~SensorNode();
  int getAddress();
  const std::string getLabel() const;
  SensorState getState();

 private:
  int address_;
  std::string label_;
  SensorState state_;
};

#endif // SENSORNODE_H_
