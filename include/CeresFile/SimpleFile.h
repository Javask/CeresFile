#pragma once
#include "File.h"

class SimpleFile : public File {
 public:
  SimpleFile(const std::filesystem::path& path);
  virtual ~SimpleFile() = default;
};