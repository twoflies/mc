// Util

#include "util.h"

std::vector<std::string> split(std::string value, char delim) {
  std::vector<std::string> components;

  std::stringstream stream;
  stream.str(value);
  std::string component;
  while (std::getline(stream, component, delim)) {
    if (!component.empty()) {
      components.push_back(component);
    }
  }
  
  return components;
}
