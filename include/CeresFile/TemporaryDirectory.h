#pragma once
#include "Directory.h"

class TemporaryDirectory : public Directory {
 public:
  TemporaryDirectory();
  virtual ~TemporaryDirectory();

 private:
  static std::filesystem::path createTempDir();
};
