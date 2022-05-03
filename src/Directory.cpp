#include "CeresFile/Directory.h"
#include <Utilities/StringUtils.h>

namespace fs = std::filesystem;
namespace CeresFile {

Directory::Directory(const std::filesystem::path& path) : dirPath(path) {
  if (!exists()) fs::create_directory(path);
}

std::vector<std::filesystem::path> Directory::getPaths() const {
  std::vector<std::filesystem::path> outVec;
  for (auto& entry : fs::directory_iterator(dirPath)) {
    outVec.push_back(entry.path());
  }
  return outVec;
}

fs::path Directory::getPath() const { return dirPath; }

bool Directory::exists() const { return fs::exists(dirPath); }

std::vector<std::filesystem::path> Directory::getDirectories() const {
  std::vector<std::filesystem::path> outVec;
  for (auto& entry : fs::directory_iterator(dirPath)) {
    if (entry.is_directory()) outVec.push_back(entry.path());
  }
  return outVec;
}

std::vector<std::filesystem::path> Directory::getFiles() const {
  std::vector<std::filesystem::path> outVec;
  for (auto& entry : fs::directory_iterator(dirPath)) {
    if (entry.is_regular_file()) outVec.push_back(entry.path());
  }
  return outVec;
}

std::vector<std::filesystem::path> Directory::getSymlinks() const {
  std::vector<std::filesystem::path> outVec;
  for (auto& entry : fs::directory_iterator(dirPath)) {
    if (entry.is_symlink()) outVec.push_back(entry.path());
  }
  return outVec;
}

std::vector<std::filesystem::path> Directory::getFilesRecursive() const {
  std::vector<std::filesystem::path> outVec;
  for (auto& entry : fs::recursive_directory_iterator(dirPath)) {
    if (entry.is_regular_file()) outVec.push_back(entry.path());
  }
  return outVec;
}

std::vector<std::filesystem::path> Directory::getPathsRecursive() const {
  std::vector<std::filesystem::path> outVec;
  for (auto& entry : fs::recursive_directory_iterator(dirPath)) {
    outVec.push_back(entry.path());
  }
  return outVec;
}

std::optional<File> Directory::createFile(std::string fileName,
                                          bool exactName) {
  auto path = dirPath;
  path.append(fileName);
  if (fs::exists(path)) {
    if (exactName) return {};
    size_t counter = 0;
    std::string stem = "";
    if (path.has_stem()) stem = path.stem().generic_string();
    std::string extension = "";
    if (path.has_extension()) extension = path.extension().generic_string();
    while (fs::exists(path)) {
      counter++;
      path = dirPath;
      path.append(stem + "(" + std::to_string(counter) + ")" + extension);
    }
  }
  return File(path);
}

std::optional<Directory> Directory::createDirectory(std::string dirName,
                                                    bool exactName) {
  auto path = dirPath;
  path.append(dirName);
  if (fs::exists(path)) {
    if (exactName) return {};
    size_t counter = 0;
    std::string stem = "";
    if (path.has_stem()) stem = path.stem().generic_string();
    std::string extension = "";
    if (path.has_extension()) extension = path.extension().generic_string();
    while (fs::exists(path)) {
      counter++;
      path = dirPath;
      path.append(stem + "(" + std::to_string(counter) + ")" + extension);
    }
  }
  return Directory(path);
}

Directory::Directory(Directory&& other) noexcept
    : dirPath(std::move(other.dirPath)) {}

Directory::Directory(const Directory& other) noexcept
    : dirPath(other.dirPath) {}

Directory& Directory::operator=(Directory&& other) noexcept {
  dirPath = std::move(other.dirPath);
  return *this;
}

Directory& Directory::operator=(const Directory& other) noexcept {
  dirPath = other.dirPath;
  return *this;
}

File Directory::createTempFile() {
  auto path = dirPath;
  path.append(Utilities::generateRandomString(8) + ".tmp");
  if (fs::exists(path)) {
    while (fs::exists(path)) {
      path = dirPath;
      path.append(Utilities::generateRandomString(8) + ".tmp");
    }
  }
  return File(path);
}

std::optional<Directory> Directory::createDirectory() {
  auto path = dirPath;
  path.append(Utilities::generateRandomString(8));
  if (fs::exists(path)) {
    while (fs::exists(path)) {
      path = dirPath;
      path.append(Utilities::generateRandomString(8));
    }
  }
  return Directory(path);
}

}  // namespace CeresFile
