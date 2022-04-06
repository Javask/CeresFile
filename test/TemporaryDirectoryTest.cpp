#include <catch2/catch.hpp>
#include "CeresFile/TemporaryDirectory.h"

namespace fs = std::filesystem;

TEST_CASE("Temporary Directory created", "[utility][temporarydir]") {
  {
    auto tempDir = TemporaryDirectory();
    REQUIRE(fs::exists(tempDir.getPath()));
  }
}

TEST_CASE("Temporary File created", "[utility][temporarydir]") {
  {
    auto tempDir = TemporaryDirectory();
    auto tmpFile = tempDir.createTempFile();
    REQUIRE(fs::exists(tmpFile));
  }
}

TEST_CASE("Named Temporary File created", "[utility][temporarydir]") {
  {
    auto tempDir = TemporaryDirectory();
    auto tmpFile = tempDir.createNewFileInDir("testFile.tmp");
    REQUIRE(fs::exists(tmpFile));
    REQUIRE(tmpFile.filename() == "testFile.tmp");
    auto tmpFile2 = tempDir.createNewFileInDir("testFile.tmp");
    REQUIRE(fs::exists(tmpFile2));
    REQUIRE(tmpFile2.filename() == "testFile(1).tmp");
    auto tmpFile3 = tempDir.createNewFileInDir("testFile");
    REQUIRE(fs::exists(tmpFile3));
    REQUIRE(tmpFile3.filename() == "testFile");
    auto tmpFile4 = tempDir.createNewFileInDir(".testFile");
    REQUIRE(fs::exists(tmpFile4));
    REQUIRE(tmpFile4.filename() == ".testFile");
  }
}
