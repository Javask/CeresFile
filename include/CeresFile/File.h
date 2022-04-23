#pragma once
#include <filesystem>
#include <istream>
#include <ostream>
#include <fstream>
#include <cstddef>
#include <vector>
#include <optional>

class File {
 public:
  File(const std::filesystem::path& path);
  virtual ~File() = default;
  virtual std::optional<std::vector<char>> read();
  virtual bool overwrite(std::vector<char> data);
  virtual bool write(std::vector<char> data, size_t offset = 0);
  virtual bool append(std::vector<char> data);

  bool operator==(const File& other);

  bool create(bool overwrite = false);
  void deleteFile();

  std::filesystem::path filename() const;
  bool exists() const;
  std::filesystem::file_time_type lastModified() const;
  size_t getFileSize() const;
  std::filesystem::path getPath() const;

 protected:
  enum FileWriteMode { APPEND, TRUNCATE, WRITE };
  enum FileOpenPos { BEGINNING, END };
  std::optional<std::ofstream> openWrite(FileWriteMode mode,
                                         int64_t offset = 0);
  std::optional<std::ifstream> openRead(FileOpenPos pos, int64_t offset = 0);

  const std::filesystem::path filePath;
};
