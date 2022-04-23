#pragma once
#include <filesystem>
#include <istream>
#include <ostream>
#include <fstream>
#include <cstddef>
#include <vector>
#include <optional>

class File {
 protected:
  File(const std::filesystem::path& path);
  virtual ~File() = default;

 public:
  virtual std::optional<std::vector<char>> read();
  virtual bool overwrite(std::vector<char> data);
  virtual bool write(std::vector<char> data, size_t offset = 0);
  virtual bool append(std::vector<char> data);

  bool create(bool overwrite = false);
  void deleteFile();

  bool exists();
  std::filesystem::file_time_type lastModified();
  size_t getFileSize();
  std::filesystem::path getPath();

 protected:
  enum FileWriteMode { APPEND, TRUNCATE, WRITE };
  enum FileOpenPos { BEGINNING, END };
  std::optional<std::ofstream> openWrite(FileWriteMode mode,
                                         int64_t offset = 0);
  std::optional<std::ifstream> openRead(FileOpenPos pos, int64_t offset = 0);

  const std::filesystem::path filePath;
};