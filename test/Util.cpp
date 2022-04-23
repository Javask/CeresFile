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

bool loopUntil(std::atomic_bool& condition,
               std::chrono::duration<int64_t, std::milli> timeout) {
  auto start = std::chrono::high_resolution_clock::now();
  while (!condition) {
    auto now = std::chrono::high_resolution_clock::now();
    if (now - start > timeout) return false;
  }
  return true;
}