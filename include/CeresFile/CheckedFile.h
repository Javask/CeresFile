#pragma once
#include "BufferedFile.h"
#include "FileAction.h"
#include "FileWatch.h"
#include <atomic>

class CheckedFile : public BufferedFile {
 public:
  CheckedFile(const std::filesystem::path& filePath);
  virtual ~CheckedFile();
  CheckedFile(const CheckedFile&) = delete;
  CheckedFile(CheckedFile&&) noexcept;
  CheckedFile& operator=(const CheckedFile&) = delete;
  CheckedFile& operator=(CheckedFile&&) noexcept;

 private:
  void onChange(const std::filesystem::path& path, FileAction);
  std::atomic_bool valid = true;
  FileWatch id;
};
