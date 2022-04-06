#pragma once
#include "Singleton.h"
#include "FileCheckerImpl.h"

class FileCheckerSingleton : public Singleton<FileCheckerSingleton> {
  friend class Singleton<FileCheckerSingleton>;
  friend class Destroyer<FileCheckerSingleton>;

 protected:
  FileCheckerSingleton() = default;
  ~FileCheckerSingleton() = default;

 public:
  void registerFileWatch(
      const std::filesystem::path& file,
      std::function<void(const std::filesystem::path&, FileAction action)>
          notify);
  void unregisterFileWatch(const std::filesystem::path& file);

 private:
  FileCheckerImpl impl;
};