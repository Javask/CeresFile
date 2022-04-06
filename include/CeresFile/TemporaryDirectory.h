#pragma once
#include <filesystem>
#include <string>

class TemporaryDirectory {
 public:
  TemporaryDirectory();
  ~TemporaryDirectory();
  std::filesystem::path createTempFile();
  std::filesystem::path createNewFileInDir(const std::string& name);
  [[nodiscard]] std::filesystem::path getPath() const;
 private:
  std::filesystem::path path_;
  static std::filesystem::path createTempDir();
};


