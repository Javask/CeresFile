#include "CeresFile/CheckedFile.h"
#include "CeresFile/FileChecker.h"

CheckedFile::CheckedFile(const std::filesystem::path& filePath)
    : BufferedFile(filePath) {
  auto callback = [this](const std::filesystem::path& path, FileAction action) {
    if (valid) onChange(path, action);
  };
  id = FileChecker::registerFileWatch(filePath, callback);
}

void CheckedFile::onChange(const std::filesystem::path& path, FileAction) {
  if (path != filePath) return;
  updateBuffer();
}

CheckedFile::~CheckedFile() {
  if (valid) FileChecker::unregisterFileWatch(id);
}

CheckedFile::CheckedFile(CheckedFile&& other) noexcept : BufferedFile(other) {
  other.valid.store(false);
  valid.store(true);
  id = other.id;
  auto callback = [this](const std::filesystem::path& path, FileAction action) {
    if (valid) onChange(path, action);
  };
  FileChecker::changeCallback(id, callback);
}
