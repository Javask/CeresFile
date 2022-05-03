#pragma once
#include "Directory.h"

namespace CeresFile {

class TemporaryDirectory : public Directory {
 public:
  TemporaryDirectory();
  virtual ~TemporaryDirectory();
};

}  // namespace CeresFile
