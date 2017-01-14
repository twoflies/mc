// Sensor Node

#include "sensornode.h"

SensorNode::SensorNode(int address, std::string label) {
  address_ = address;
  label_ = label;
  state_ = SENSOR_STATE_UNKNOWN;
}

SensorNode::~SensorNode() {
}

int SensorNode::getAddress() {
  return address_;
}

const std::string SensorNode::getLabel() const {
  return label_;
}

SensorState SensorNode::getState() {
  return state_;
}
