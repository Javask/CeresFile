#include "FileCheckerSingleton.h"
#include "FileCheckerImpl.h"

FileWatch FileCheckerSingleton::registerFileWatch(
    const std::filesystem::path& file, FileCallback notify) {
  return impl.registerFileWatch(file, notify);
}

void FileCheckerSingleton::addFileToWatch(FileWatch id,
                                          const std::filesystem::path& file) {
  impl.addFileToWatch(id, file);
}

void FileCheckerSingleton::changeCallback(FileWatch id, FileCallback notify) {
  impl.changeCallback(id, notify);
}

void FileCheckerSingleton::unregisterFileFromWatch(
    FileWatch id, const std::filesystem::path& file) {
  impl.unregisterFileFromWatch(id, file);
}

void FileCheckerSingleton::unregisterFileWatch(FileWatch id) {
  impl.unregisterFileWatch(id);
}
