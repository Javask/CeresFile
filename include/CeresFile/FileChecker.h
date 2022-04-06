#pragma once
#include <filesystem>
#include <functional>
#include "FileAction.h"

class FileChecker {
 private:
  FileChecker() = default;
  ~FileChecker() = default;

 public:
  static void registerFileWatch(
      const std::filesystem::path& file,
      std::function<void(const std::filesystem::path&, FileAction action)>
          notify);
  static void unregisterFileWatch(const std::filesystem::path& file);
};