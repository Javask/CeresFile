#pragma once
#include "File.h"
#include <mutex>

namespace CeresFile {

class BufferedFile : public File {
 public:
  BufferedFile(const std::filesystem::path& path, bool onlyWriteLocal = false);
  BufferedFile(const File& base, bool onlyWriteLocal = false) noexcept;
  BufferedFile(File&& base, bool onlyWriteLocal = false) noexcept;
  BufferedFile(const BufferedFile& other) noexcept;
  BufferedFile(BufferedFile&& other) noexcept;
  virtual ~BufferedFile() = default;

  BufferedFile& operator=(const File& base) noexcept = delete;
  BufferedFile& operator=(File&& base) noexcept = delete;

  std::optional<std::vector<char>> read() override;

  bool overwrite(std::vector<char> data) override;
  bool write(std::vector<char> data, size_t offset = 0) override;
  bool append(std::vector<char> data) override;

 protected:
  const bool onlyWriteLocal;

  mutable std::mutex mutex;
  std::filesystem::file_time_type lastModifiedTime;
  bool doesExist;
  size_t currFileSize;

  std::vector<char> data;

  void updateBuffer();
  void updateState();
  void writeToBuffer(std::vector<char> data, size_t offset = 0);

 private:
  BufferedFile(bool onlyWriteLocal);
};

}  // namespace CeresFile