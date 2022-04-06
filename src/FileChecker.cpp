#include "CeresFile/FileChecker.h"
#include "FileCheckerSingleton.h"

  void FileChecker::registerFileWatch(
    const std::filesystem::path& file,
    std::function<void(const std::filesystem::path&, FileAction action)>
        notify) {
  FileCheckerSingleton::Get()->registerFileWatch(file, notify);
}

  void FileChecker::unregisterFileWatch(const std::filesystem::path& file) {
    FileCheckerSingleton::Get()->unregisterFileWatch(file);
  }