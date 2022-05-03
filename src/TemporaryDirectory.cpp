#include "../include/CeresFile/TemporaryDirectory.h"
#include <string>
#include <ostream>
#include <istream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <Utilities/FileUtils.h>

namespace fs = std::filesystem;
namespace CeresFile {

TemporaryDirectory::TemporaryDirectory()
    : Directory(Utilities::createTempDir()) {}

TemporaryDirectory::~TemporaryDirectory() { fs::remove_all(dirPath); }

}  // namespace CeresFile