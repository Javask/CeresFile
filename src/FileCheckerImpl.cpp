#include "FileCheckerImpl.h"

namespace fs = std::filesystem;

FileCheckerImpl::FileCheckerImpl() {
  checkerThread = std::thread([this]() { this->pCheckerMain(); });
}

FileCheckerImpl::~FileCheckerImpl() {
  stop.store(true);
  checkerThread.join();
}

void FileCheckerImpl::registerFileWatch(
    const std::filesystem::path& file,
    std::function<void(const std::filesystem::path&, FileAction action)>
        notify) {
  auto lock = std::unique_lock(mapGuard);
  if (map.contains(file)) return;
  FileInfo info;
  info.exists = fs::exists(file);
  if (info.exists) {
    info.lastModified = fs::last_write_time(file);
  }
  info.callback = notify;
  map.emplace(file, info);
}

void FileCheckerImpl::unregisterFileWatch(const std::filesystem::path& file) {
  auto lock = std::unique_lock(mapGuard);
  if (!map.contains(file)) return;
  map.erase(file);
}

void FileCheckerImpl::checkFile(const std::filesystem::path& file,
                                FileInfo& info) {
  auto exists = fs::exists(file);
  fs::file_time_type lastModified;
  if (exists) lastModified = fs::last_write_time(file);
  if (exists != info.exists) {
    info.exists = exists;
    if (info.exists) {
      info.lastModified = lastModified;
    }
    info.callback(file, exists ? FileAction::CREATED : FileAction::DELETED);
  } else if (exists && lastModified != info.lastModified) {
    info.lastModified = lastModified;
    info.callback(file, FileAction::MODIFIED);
  }
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
          checkFile(pair.first, pair.second);
        }
      }
    }

    std::this_thread::sleep_for(timeStep);
  }
}
