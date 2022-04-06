#include "../include/CeresFile/TemporaryDirectory.h"
#include "RandomString.h"
#include <string>
#include <ostream>
#include <istream>
#include <fstream>
#include <cassert>
#include <cstring>

namespace fs = std::filesystem;

TemporaryDirectory::TemporaryDirectory()
    :  path_(createTempDir()) {}

fs::path TemporaryDirectory::createTempDir() {
  const auto TempDir = fs::temp_directory_path();
  auto dir = fs::path(TempDir);
  while (exists(dir)) {
    dir = fs::path(TempDir).append(generateRandomString(8));
  }
  assert(create_directory(dir));
  return dir;
}

TemporaryDirectory::~TemporaryDirectory() {
  remove_all(path_);
}

 std::filesystem::path TemporaryDirectory::createTempFile() {
  return createNewFileInDir(generateRandomString(8)+".tmp");
}

 std::filesystem::path TemporaryDirectory::createNewFileInDir(
    const std::string& name) {
  auto outPath = fs::path(path_).append(name);
   auto stem = outPath.stem().generic_string();
  auto extension = outPath.extension().generic_string();
  int counter = 0;
  while (fs::exists(outPath)) {
    counter++;
    outPath = fs::path(path_).append(stem + "(" + std::to_string(counter) +
                                     ")" + extension);
  }
  auto stream = std::ofstream(outPath);
  stream.close();
  return outPath;
}

 std::filesystem::path TemporaryDirectory::getPath() const { return path_; }
