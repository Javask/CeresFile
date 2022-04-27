#include "../include/CeresFile/TemporaryDirectory.h"
#include "RandomString.h"
#include <string>
#include <ostream>
#include <istream>
#include <fstream>
#include <cassert>
#include <cstring>

namespace fs = std::filesystem;
namespace CeresFile {

TemporaryDirectory::TemporaryDirectory() : Directory(createTempDir()) {}

fs::path TemporaryDirectory::createTempDir() {
  const auto TempDir = fs::temp_directory_path();
  auto dir = fs::path(TempDir);
  while (fs::exists(dir)) {
    dir = fs::path(TempDir).append(generateRandomString(8));
  }
  assert(create_directory(dir));
  return dir;
}

TemporaryDirectory::~TemporaryDirectory() { fs::remove_all(dirPath); }

}  // namespace CeresFile