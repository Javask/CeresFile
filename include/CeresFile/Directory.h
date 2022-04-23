#pragma once
#include <filesystem>
#include "File.h"
#include <optional>

class Directory {
 public:
  Directory(const std::filesystem::path& path);
  virtual ~Directory() = default;
  Directory(const Directory& other) noexcept;
  Directory(Directory&& other) noexcept;

  Directory& operator=(Directory&& other) noexcept;
  Directory& operator=(const Directory& other) noexcept;

  std::vector<std::filesystem::path> getPaths() const;
  std::vector<std::filesystem::path> getPathsRecursive() const;
  std::vector<std::filesystem::path> getFiles() const;
  std::vector<std::filesystem::path> getFilesRecursive() const;
  std::vector<std::filesystem::path> getDirectories() const;
  std::vector<std::filesystem::path> getSymlinks() const;

  bool exists() const;
  std::filesystem::path getPath() const;

  std::optional<File> createFile(std::string fileName, bool exactName = false);
  File createTempFile();
  std::optional<Directory> createDirectory(std::string dirName,
                                           bool exactName = false);
  std::optional<Directory> createDirectory();

 protected:
  std::filesystem::path dirPath;
};
