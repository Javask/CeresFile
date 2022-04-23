#include "../include/CeresFile/SimpleFile.h"

namespace fs = std::filesystem;

SimpleFile::SimpleFile(const std::filesystem::path& path) : File(path) {}
