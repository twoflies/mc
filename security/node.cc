// Node

#include "node.h"

Node::Node() {
  online_ = false;
}

Node::~Node() {
}

bool Node::isOnline() {
  return online_;
}

