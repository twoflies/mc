// Util

#include "util.h"

#include <iomanip>

std::vector<std::string> split(const std::string& value, char delim) {
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

void readHexString(std::basic_istream<char>& stream, byte* data, int length) {
  for (int index = 0; index < length; index++) {
    int value;
    stream >> std::hex >> value;
    data[index] = (byte)value;
  }
}

void writeHexString(std::basic_ostream<char>& stream, byte* data, int length) {
  for (int index = 0; index < length; index++) {
    stream << std::setw(2) << std::setfill('0') << std::hex << (int)data[index];
    if (index < (length -1)) {
      stream << " ";
    }
  }
}
