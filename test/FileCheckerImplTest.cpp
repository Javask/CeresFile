#include "FileCheckerImpl.h"
#include <catch2/catch.hpp>
#include "CeresFile/TemporaryDirectory.h"
#include <condition_variable>
#include <mutex>
#include <ostream>
#include <fstream>

TEST_CASE("FileCheckerImpl Test", "[file][thread]") {
  {
    auto tmpDir = TemporaryDirectory();
    auto tmpFilePath = tmpDir.createTempFile();
    std::atomic_bool isReady = false;
    std::atomic_bool hasBeenCreated = false;
    std::atomic_bool hasBeenDeleted = false;
    std::atomic_bool hasBeenModified = false;
    std::atomic_bool correctFile = false;

    auto callback = [&hasBeenCreated, &hasBeenDeleted, &hasBeenModified,
                     tmpFilePath, &correctFile, &isReady](const std::filesystem::path& file,
                                                FileAction action) {
      switch (action) {
        case FileAction::CREATED:
          hasBeenCreated.store(true);
          break;
        case FileAction::DELETED:
          hasBeenDeleted.store(true);
          break;
        case FileAction::MODIFIED:
          hasBeenModified.store(true);
          break;
        default:
          break;
      }
      if (file.generic_string() == tmpFilePath.generic_string()) correctFile.store(true);
      isReady.store(true);
    };

    auto fileChecker = FileCheckerImpl();
    fileChecker.registerFileWatch(tmpFilePath,callback);
    std::filesystem::remove(tmpFilePath);
    while (!isReady) {
    }

    REQUIRE(correctFile);
    REQUIRE(!hasBeenCreated);
    REQUIRE(hasBeenDeleted);
    REQUIRE(!hasBeenModified);

    isReady.store(false);
    hasBeenDeleted.store(false);
    correctFile.store(false);
    auto outStream = std::ofstream(tmpFilePath.generic_string());
    outStream.close();
    while (!isReady) {
    }

    REQUIRE(correctFile);
    REQUIRE(hasBeenCreated);
    REQUIRE(!hasBeenDeleted);
    REQUIRE(!hasBeenModified);

    isReady.store(false);
    hasBeenCreated.store(false);
    correctFile.store(false);
    outStream = std::ofstream(tmpFilePath.generic_string());
    outStream.write("test", 4);
    outStream.close();
    while (!isReady) {
    }
    REQUIRE(correctFile);
    REQUIRE(!hasBeenCreated);
    REQUIRE(!hasBeenDeleted);
    REQUIRE(hasBeenModified);
  }
}