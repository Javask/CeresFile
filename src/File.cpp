#include "CeresFile/File.h"

namespace fs = std::filesystem;

File::File(const std::filesystem::path& path) : filePath(path) {
  if (!exists()) create();
}

std::optional<std::ofstream> File::openWrite(FileWriteMode mode,
                                             int64_t offset) {
  if (!fs::exists(filePath)) return {};
  std::ios::openmode openMode = std::ios::out | std::ios::binary;
  if (mode == FileWriteMode::TRUNCATE) openMode |= std::ios::trunc;
  if (mode == FileWriteMode::APPEND) openMode |= std::ios::app;
  auto stream = std::ofstream(filePath.generic_string(), openMode);
  if (!stream.is_open()) return {};
  stream.seekp(offset, std::ios::cur);
  if (stream.fail()) {
    stream.close();
    return {};
  }
  return stream;
}

std::optional<std::ifstream> File::openRead(FileOpenPos pos, int64_t offset) {
  if (!fs::exists(filePath)) return {};

  std::ios::openmode openMode = std::ios::in | std::ios::binary;
  if (pos == FileOpenPos::END) openMode |= std::ios::ate;
  auto stream = std::ifstream(filePath.generic_string(), openMode);
  if (!stream.is_open()) return {};
  stream.seekg(offset, std::ios::cur);
  if (stream.fail()) {
    stream.close();
    return {};
  }
  return stream;
}

std::optional<std::vector<char>> File::read() {
  auto streamOpt = openRead(FileOpenPos::END);
  if (!streamOpt) return {};
  auto fileEnd = streamOpt->tellg();
  streamOpt->seekg(0, std::ios::beg);
  auto fileSize = fileEnd - streamOpt->tellg();
  auto data = std::vector<char>(fileSize);
  streamOpt->read(data.data(), fileSize);
  streamOpt->close();
  return data;
}

bool File::overwrite(std::vector<char> data) {
  auto streamOpt = openWrite(FileWriteMode::TRUNCATE);
  if (!streamOpt) return {};
  streamOpt->write(data.data(), data.size());
  auto fail = streamOpt->fail();
  streamOpt->close();
  return !fail;
}

bool File::write(std::vector<char> data, size_t offset) {
  auto streamOpt =
      openWrite(FileWriteMode::WRITE, static_cast<int64_t>(offset));
  if (!streamOpt) return {};
  if (streamOpt->fail()) {
    streamOpt->close();
    return false;
  }
  streamOpt->write(data.data(), data.size());
  auto fail = streamOpt->fail();
  streamOpt->close();
  return !fail;
}

bool File::append(std::vector<char> data) {
  auto streamOpt = openWrite(FileWriteMode::APPEND);
  if (!streamOpt) return {};
  streamOpt->write(data.data(), data.size());
  auto fail = streamOpt->fail();
  streamOpt->close();
  return !fail;
}

bool File::create(bool overwrite) {
  if (exists() && !overwrite) return false;
  std::ios::openmode openMode = std::ios::out | std::ios::binary;
  if (overwrite) openMode |= std::ios::trunc;
  auto stream = std::ofstream(filePath.generic_string(), openMode);
  if (!stream.is_open()) return false;
  stream.close();
  return true;
}

void File::deleteFile() { fs::remove(filePath); }

bool File::exists() const { return fs::exists(filePath); }

std::filesystem::file_time_type File::lastModified() const {
  return fs::last_write_time(filePath);
}

size_t File::getFileSize() const { return fs::file_size(filePath); }

std::filesystem::path File::getPath() const { return filePath; }

bool File::operator==(const File& other) { return filePath == other.filePath; }

std::filesystem::path File::filename() const { return filePath.filename(); }