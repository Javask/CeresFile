#include "Util.h"
std::vector<char> stringToVector(const std::string& message) {
  auto data = std::vector<char>(message.size());
  for (size_t i = 0; i < data.size(); i++) {
    data[i] = message.c_str()[i];
  }
  return data;
}

std::string vectorToString(const std::vector<char>& message) {
  return std::string(message.data(), message.size());
}