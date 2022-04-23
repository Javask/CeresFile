#include "CeresFile/CheckedFile.h"
#include "CeresFile/FileChecker.h"

CheckedFile::CheckedFile(const std::filesystem::path& filePath,
                         bool onlyWriteLocal)
    : BufferedFile(filePath, onlyWriteLocal) {
  id = FileChecker::registerFileWatch(filePath, getCallback());
}

CheckedFile::CheckedFile(const File& base, bool onlyWriteLocal) noexcept
    : BufferedFile(base, onlyWriteLocal) {
  id = FileChecker::registerFileWatch(filePath, getCallback());
}

CheckedFile::CheckedFile(const BufferedFile& base) noexcept
    : BufferedFile(base) {
  id = FileChecker::registerFileWatch(filePath, getCallback());
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

  FileChecker::changeCallback(id, getCallback());
}

FileCallback CheckedFile::getCallback() {
  return [this](const std::filesystem::path& path, FileAction action) {
    if (valid) onChange(path, action);
  };
}
