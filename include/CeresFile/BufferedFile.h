#pragma once
#include "File.h"
#include <mutex>

class BufferedFile : public File {
 public:
  BufferedFile(const std::filesystem::path& path, bool onlyWriteLocal = false);
  virtual ~BufferedFile() = default;

  std::optional<std::vector<char>> read() override;
  bool overwrite(std::vector<char> data) override;
  bool write(std::vector<char> data, size_t offset = 0) override;
  bool append(std::vector<char> data) override;

 private:
  std::mutex mutex;
  const bool onlyWriteLocal;
  std::filesystem::file_time_type lastModifiedTime;
  bool doesExist;
  size_t currFileSize;
  std::vector<char> data;
  void updateBuffer();
  void writeToBuffer(std::vector<char> data, size_t offset = 0);
};
