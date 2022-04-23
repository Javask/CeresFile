#pragma once
#include "Singleton.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <filesystem>
#include <functional>
#include <map>
#include <condition_variable>
#include "CeresFile/FileAction.h"

struct FileInfo {
  bool exists;
  size_t fileSize;
  std::filesystem::file_time_type lastModified;
  std::function<void(const std::filesystem::path&, FileAction action)> callback;
};

class FileCheckerImpl {
 public:
  FileCheckerImpl();
  virtual ~FileCheckerImpl();
  void registerFileWatch(
      const std::filesystem::path& file,
      std::function<void(const std::filesystem::path&, FileAction action)>
          notify);
  void unregisterFileWatch(const std::filesystem::path& file);

 private:
  static void checkFile(const std::filesystem::path& file, FileInfo& info);
  void pCheckerMain();
  std::atomic_bool stop = false;
  const std::chrono::milliseconds timeStep = std::chrono::milliseconds(100);
  std::thread checkerThread;
  std::mutex mapGuard;
  std::condition_variable mapCV;
  std::map<std::filesystem::path, FileInfo> map;
};