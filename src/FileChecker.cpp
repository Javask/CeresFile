#include "CeresFile/FileChecker.h"
#include "FileCheckerSingleton.h"

FileWatch FileChecker::registerFileWatch(
    const std::filesystem::path& file,
    std::function<void(const std::filesystem::path&, FileAction action)>
        notify) {
  return FileCheckerSingleton::Get()->registerFileWatch(file, notify);
}

void FileChecker::unregisterFileWatch(FileWatch id) {
  FileCheckerSingleton::Get()->unregisterFileWatch(id);
}

void FileChecker::addFileToWatch(FileWatch id,
                                 const std::filesystem::path& file) {
  FileCheckerSingleton::Get()->addFileToWatch(id, file);
}

void FileChecker::changeCallback(FileWatch id, FileCallback notify) {
  FileCheckerSingleton::Get()->changeCallback(id, notify);
}

void FileChecker::unregisterFileFromWatch(FileWatch id,
                                          const std::filesystem::path& file) {
  FileCheckerSingleton::Get()->unregisterFileFromWatch(id, file);
}