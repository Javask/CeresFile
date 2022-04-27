#include "FileCheckerImpl.h"

namespace fs = std::filesystem;

namespace CeresFile {

FileCheckerImpl::FileCheckerImpl() {
  checkerThread = std::thread([this]() { this->pCheckerMain(); });
}

FileCheckerImpl::~FileCheckerImpl() {
  stop.store(true);
  checkerThread.join();
}

FileWatch FileCheckerImpl::registerFileWatch(const std::filesystem::path& file,
                                             FileCallback notify) {
  auto lock = std::unique_lock(mapGuard);
  auto id = getNextID();
  ids[file].insert(id);
  callbacks[id] = notify;
  if (!map.contains(file)) {
    addFileToMap(file);
  }
  return id;
}

void FileCheckerImpl::addFileToWatch(FileWatch id,
                                     const std::filesystem::path& file) {
  auto lock = std::unique_lock(mapGuard);
  if (!callbacks.contains(id)) return;
  ids[file].insert(id);
  if (!map.contains(file)) {
    addFileToMap(file);
  }
}

void FileCheckerImpl::changeCallback(FileWatch id, FileCallback notify) {
  auto lock = std::unique_lock(mapGuard);
  if (callbacks.contains(id)) callbacks[id] = notify;
}

void FileCheckerImpl::unregisterFileWatch(FileWatch id) {
  auto lock = std::unique_lock(mapGuard);
  std::vector<std::filesystem::path> toRemove;
  for (auto fileNamePair : ids) {
    if (fileNamePair.second.contains(id)) {
      fileNamePair.second.erase(id);
      if (fileNamePair.second.empty()) {
        toRemove.push_back(fileNamePair.first);
      }
    }
  }
  for (auto file : toRemove) {
    ids.erase(file);
    map.erase(file);
  }
  if (callbacks.contains(id)) callbacks.erase(id);
}

void FileCheckerImpl::unregisterFileFromWatch(
    FileWatch id, const std::filesystem::path& file) {
  auto lock = std::unique_lock(mapGuard);
  bool isOnlyFile = true;
  std::vector<std::filesystem::path> toRemove;
  for (auto fileNamePair : ids) {
    if (fileNamePair.second.contains(id)) {
      if (fileNamePair.first == file) {
        fileNamePair.second.erase(id);
        if (fileNamePair.second.empty()) {
          toRemove.push_back(fileNamePair.first);
        }
      } else {
        isOnlyFile = false;
      }
    }
  }
  for (auto file : toRemove) {
    ids.erase(file);
    map.erase(file);
  }
  if (isOnlyFile && callbacks.contains(id)) callbacks.erase(id);
}

void FileCheckerImpl::checkFile(const std::filesystem::path& file) {
  auto idsForFile = ids[file];
  auto& info = map[file];
  auto callbacksForFile = std::vector<FileCallback>();
  for (auto id : idsForFile) {
    if (callbacks.contains(id)) callbacksForFile.push_back(callbacks[id]);
  }

  auto exists = fs::exists(file);
  fs::file_time_type lastModified;
  size_t fileSize;
  if (exists) {
    lastModified = fs::last_write_time(file);
    fileSize = fs::file_size(file);
  }
  if (exists != info.exists) {
    info.exists = exists;
    if (info.exists) {
      info.lastModified = lastModified;
      info.fileSize = fileSize;
    }
    for (auto& c : callbacksForFile)
      c(file, exists ? FileAction::CREATED : FileAction::DELETED);

  } else if (exists &&
             (lastModified != info.lastModified || fileSize != info.fileSize)) {
    info.lastModified = lastModified;
    info.fileSize = fileSize;
    for (auto& c : callbacksForFile) c(file, FileAction::MODIFIED);
  }
}

void FileCheckerImpl::addFileToMap(const std::filesystem::path& file) {
  FileInfo info;
  info.exists = fs::exists(file);
  if (info.exists) {
    info.lastModified = fs::last_write_time(file);
    info.fileSize = fs::file_size(file);
  }
  map.emplace(file, info);
}

void FileCheckerImpl::pCheckerMain() {
  while (!stop) {
    {
      auto lock = std::unique_lock(mapGuard);
      if (map.empty()) {
        mapCV.wait_for(lock, timeStep);
      }
      if (!map.empty()) {
        for (auto& pair : map) {
          checkFile(pair.first);
        }
      }
    }

    std::this_thread::sleep_for(timeStep);
  }
}

FileWatch FileCheckerImpl::getNextID() {
  static FileWatch id = 0;
  return id++;
}

}  // namespace CeresFile