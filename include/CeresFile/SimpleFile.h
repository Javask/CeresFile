#pragma once
#include <filesystem>
#include <istream>
#include <ostream>
#include <fstream>
#include <cstddef>
#include <vector>
#include <optional>

enum FileWriteMode { APPEND, TRUNCATE, WRITE };
enum FileOpenPos { BEGINNING, END };

class SimpleFile {
 public:
  SimpleFile(const std::filesystem::path& path);
  virtual ~SimpleFile() = default;

 public:
  std::optional<std::ofstream> openWrite(FileWriteMode mode,
                                         int64_t offset = 0);
  std::optional<std::ifstream> openRead(FileOpenPos pos, int64_t offset = 0);

  std::optional<std::vector<char>> read();
  bool overwrite(std::vector<char> data);
  bool write(std::vector<char> data, int64_t offset = 0);
  bool append(std::vector<char> data);

  bool create(bool overwrite = false);
  void deleteFile();

  bool exists();
  std::filesystem::file_time_type lastModified();
  size_t hash();
  std::filesystem::path getPath();

 private:
  const std::filesystem::path filePath;
};