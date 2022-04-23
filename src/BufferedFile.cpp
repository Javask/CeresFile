#include "CeresFile/BufferedFile.h"
#include <cstring>
BufferedFile::BufferedFile(const std::filesystem::path& path,
                           bool onlyWriteLocal)
    : File(path), onlyWriteLocal(onlyWriteLocal) {
  doesExist = exists();
  if (exists()) {
    auto readData = read();
    if (readData) data = *readData;
    lastModifiedTime = lastModified();
    currFileSize = getFileSize();
  }
}

void BufferedFile::updateBuffer() {
  auto newExists = exists();
  if (!newExists) {
    doesExist = false;
    data.clear();
    return;
  }
  auto newModifies = lastModified();
  auto newFileSize = getFileSize();

  if (doesExist != newExists || lastModifiedTime != newModifies ||
      currFileSize != newFileSize) {
    auto readOpt = File::read();
    if (readOpt) {
      doesExist = newExists;
      lastModifiedTime = newModifies;
      currFileSize = newFileSize;
      data = std::move(*readOpt);
    }
  }
}

void BufferedFile::updateState() {
  doesExist = exists();
  lastModifiedTime = lastModified();
  currFileSize = getFileSize();
}

void BufferedFile::writeToBuffer(std::vector<char> inData, size_t offset) {
  size_t requiredSize = inData.size() + offset;
  if (requiredSize > data.size()) data.resize(requiredSize);
  memcpy(data.data() + offset, inData.data(), inData.size());
}

bool BufferedFile::write(std::vector<char> inData, size_t offset) {
  auto lock = std::unique_lock(mutex);
  if (!onlyWriteLocal && !File::write(inData, offset)) return false;
  writeToBuffer(inData, offset);
  updateState();
  return true;
}

std::optional<std::vector<char>> BufferedFile::read() {
  auto lock = std::unique_lock(mutex);
  updateBuffer();
  if (doesExist)
    return data;
  else
    return {};
}

bool BufferedFile::overwrite(std::vector<char> inData) {
  auto lock = std::unique_lock(mutex);
  if (!onlyWriteLocal && !File::overwrite(inData)) return false;
  data = inData;
  updateState();
  return true;
}

bool BufferedFile::append(std::vector<char> inData) {
  auto lock = std::unique_lock(mutex);
  if (!onlyWriteLocal && !File::append(inData)) return false;
  writeToBuffer(inData, data.size());
  updateState();
  return true;
}

BufferedFile::BufferedFile(const File& base, bool onlyWriteLocal) noexcept
    : BufferedFile(base.getPath(), onlyWriteLocal) {}

BufferedFile::BufferedFile(File&& base, bool onlyWriteLocal) noexcept
    : BufferedFile(base.getPath(), onlyWriteLocal) {}

BufferedFile::BufferedFile(BufferedFile&& other) noexcept
    : File(other), onlyWriteLocal(other.onlyWriteLocal) {
  auto lock = std::unique_lock(other.mutex);
  data = std::move(other.data);
  lastModifiedTime = std::move(other.lastModifiedTime);
  doesExist = std::move(other.doesExist);
  currFileSize = std::move(other.currFileSize);
}

BufferedFile::BufferedFile(const BufferedFile& other) noexcept
    : File(other), onlyWriteLocal(other.onlyWriteLocal) {
  auto lock = std::unique_lock(other.mutex);
  data = other.data;
  lastModifiedTime = other.lastModifiedTime;
  doesExist = other.doesExist;
  currFileSize = other.currFileSize;
}