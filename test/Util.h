#pragma once
#include <string>
#include <vector>
#include <atomic>
#include <chrono>

bool loopUntil(std::atomic_bool& condition,
               std::chrono::duration<int64_t, std::milli> timeout);

std::vector<char> stringToVector(const std::string& message);

std::string vectorToString(const std::vector<char>& message);