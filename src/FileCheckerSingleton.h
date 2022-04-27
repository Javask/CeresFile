#pragma once
#include "Singleton.h"
#include "FileCheckerImpl.h"

namespace CeresFile {

class FileCheckerSingleton : public Singleton<FileCheckerSingleton> {
  friend class Singleton<FileCheckerSingleton>;
  friend class Destroyer<FileCheckerSingleton>;

 protected:
  FileCheckerSingleton() = default;
  ~FileCheckerSingleton() = default;

 public:
  FileWatch registerFileWatch(const std::filesystem::path& file,
                              FileCallback notify);
  void addFileToWatch(FileWatch id, const std::filesystem::path& file);
  void changeCallback(FileWatch id, FileCallback notify);
  void unregisterFileFromWatch(FileWatch id, const std::filesystem::path& file);
  void unregisterFileWatch(FileWatch id);

 private:
  FileCheckerImpl impl;
};

}  // namespace CeresFile
