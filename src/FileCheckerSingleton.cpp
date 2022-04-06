#include "FileCheckerSingleton.h"
#include "FileCheckerImpl.h"

void FileCheckerSingleton::registerFileWatch(
    const std::filesystem::path& file,
    std::function<void(const std::filesystem::path&, FileAction action)>
        notify) {
  impl.registerFileWatch(file, notify);
}

void FileCheckerSingleton::unregisterFileWatch(
    const std::filesystem::path& file) {
  impl.unregisterFileWatch(file);
}
