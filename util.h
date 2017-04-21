#ifndef UTIL_H_
#define UTIL_H_

// Util

#include <vector>
#include <string>
#include <sstream>
#include <istream>
#include <ostream>
#include "xbserial/file.h"

std::vector<std::string> split(const std::string& value, char delim);
void readHexString(std::basic_istream<char>& stream, byte* data, int length);
void writeHexString(std::basic_ostream<char>& stream, byte* data, int length);

#endif // UTIL_H_
