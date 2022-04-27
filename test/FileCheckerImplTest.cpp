#include "FileCheckerImpl.h"
#include <catch2/catch.hpp>
#include "CeresFile/TemporaryDirectory.h"
#include <condition_variable>
#include <mutex>
#include <ostream>
#include <fstream>
#include "Util.h"

using namespace CeresFile;
TEST_CASE("FileCheckerImpl Test", "[file][thread]") {
  {
    auto timeout = std::chrono::seconds(10);
    auto tmpDir = TemporaryDirectory();
    auto tmpFilePath = tmpDir.createTempFile().getPath();
    auto tmpFilePath2 = tmpDir.createTempFile().getPath();
    std::atomic_bool isReady = false;
    std::atomic_bool hasBeenCreated = false;
    std::atomic_bool hasBeenDeleted = false;
    std::atomic_bool hasBeenModified = false;
    std::atomic_bool correctFile = false;

    auto callback = [&hasBeenCreated, &hasBeenDeleted, &hasBeenModified,
                     tmpFilePath, &correctFile, &isReady](
                        const std::filesystem::path& file, FileAction action) {
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
      auto fStr = file.generic_string();
      auto tStr = tmpFilePath.generic_string();
      if (fStr == tStr) correctFile.store(true);
      isReady.store(true);
    };

    auto callback2 = [&hasBeenCreated, &hasBeenDeleted, &hasBeenModified,
                      tmpFilePath2, &correctFile, &isReady](
                         const std::filesystem::path& file, FileAction action) {
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
      auto fStr = file.generic_string();
      auto tStr = tmpFilePath2.generic_string();
      if (fStr == tStr) correctFile.store(true);
      isReady.store(true);
    };

    auto fileChecker = FileCheckerImpl();
    auto id = fileChecker.registerFileWatch(tmpFilePath, callback);
    std::filesystem::remove(tmpFilePath);
    REQUIRE(loopUntil(isReady, timeout));

    REQUIRE(correctFile);
    REQUIRE(!hasBeenCreated);
    REQUIRE(hasBeenDeleted);
    REQUIRE(!hasBeenModified);

    isReady.store(false);
    hasBeenDeleted.store(false);
    correctFile.store(false);
    auto outStream = std::ofstream(tmpFilePath.generic_string());
    outStream.close();
    REQUIRE(loopUntil(isReady, timeout));

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
    REQUIRE(loopUntil(isReady, timeout));

    REQUIRE(correctFile);
    REQUIRE(!hasBeenCreated);
    REQUIRE(!hasBeenDeleted);
    REQUIRE(hasBeenModified);

    isReady.store(false);
    hasBeenModified.store(false);
    hasBeenCreated.store(false);
    hasBeenDeleted.store(false);
    correctFile.store(false);

    fileChecker.changeCallback(id, callback2);
    std::filesystem::remove(tmpFilePath);
    REQUIRE(loopUntil(isReady, timeout));

    REQUIRE(!correctFile);
    REQUIRE(!hasBeenCreated);
    REQUIRE(hasBeenDeleted);
    REQUIRE(!hasBeenModified);

    isReady.store(false);
    hasBeenDeleted.store(false);

    fileChecker.addFileToWatch(id, tmpFilePath2);
    std::filesystem::remove(tmpFilePath2);
    REQUIRE(loopUntil(isReady, timeout));

    REQUIRE(correctFile);
    REQUIRE(!hasBeenCreated);
    REQUIRE(hasBeenDeleted);
    REQUIRE(!hasBeenModified);

    isReady.store(false);
    correctFile.store(false);
    hasBeenDeleted.store(false);

    fileChecker.unregisterFileFromWatch(id, tmpFilePath);
    outStream = std::ofstream(tmpFilePath.generic_string());
    outStream.write("test", 4);
    outStream.close();
    REQUIRE(!loopUntil(isReady, std::chrono::milliseconds(100)));
  }
}
