#pragma once
#include <Utilities/Singleton.h>
#include "FileCheckerImpl.h"

namespace CeresFile {

class FileCheckerSingleton : public Utilities::Singleton<FileCheckerSingleton> {
  friend class Utilities::Singleton<FileCheckerSingleton>;
  friend class Utilities::Destroyer<FileCheckerSingleton>;

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
