#pragma once
#include "Singleton.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <filesystem>
#include <functional>
#include <map>
#include <set>
#include <condition_variable>
#include "CeresFile/FileCallback.h"
#include "CeresFile/FileWatch.h"

struct FileInfo {
  bool exists;
  size_t fileSize;
  std::filesystem::file_time_type lastModified;
};

class FileCheckerImpl {
 public:
  FileCheckerImpl();
  virtual ~FileCheckerImpl();
  FileWatch registerFileWatch(const std::filesystem::path& file,
                              FileCallback notify);
  void addFileToWatch(FileWatch id, const std::filesystem::path& file);
  void changeCallback(FileWatch id, FileCallback notify);
  void unregisterFileWatch(FileWatch id);
  void unregisterFileFromWatch(FileWatch id, const std::filesystem::path& file);

 private:
  void checkFile(const std::filesystem::path& file);
  void addFileToMap(const std::filesystem::path& file);
  void pCheckerMain();
  std::atomic_bool stop = false;
  const std::chrono::milliseconds timeStep = std::chrono::milliseconds(100);
  std::thread checkerThread;
  std::mutex mapGuard;
  std::condition_variable mapCV;
  std::map<std::filesystem::path, FileInfo> map;
  std::map<std::filesystem::path, std::set<FileWatch>> ids;
  std::map<FileWatch, FileCallback> callbacks;
  static FileWatch getNextID();
};