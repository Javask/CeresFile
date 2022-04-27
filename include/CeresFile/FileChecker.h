#pragma once
#include <filesystem>
#include <functional>
#include "FileCallback.h"
#include "FileWatch.h"

namespace CeresFile {

class FileChecker {
 private:
  FileChecker() = default;
  ~FileChecker() = default;

 public:
  static FileWatch registerFileWatch(const std::filesystem::path& file,
                                     FileCallback notify);
  static void unregisterFileWatch(FileWatch id);
  static void addFileToWatch(FileWatch id, const std::filesystem::path& file);
  static void changeCallback(FileWatch id, FileCallback notify);
  static void unregisterFileFromWatch(FileWatch id,
                                      const std::filesystem::path& file);
};

}  // namespace CeresFile