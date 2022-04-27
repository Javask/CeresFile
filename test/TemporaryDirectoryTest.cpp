#include <catch2/catch.hpp>
#include "CeresFile/TemporaryDirectory.h"

namespace fs = std::filesystem;
using namespace CeresFile;

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
    REQUIRE(tmpFile.exists());
  }
}

TEST_CASE("Named Temporary File created", "[utility][temporarydir]") {
  {
    auto tempDir = TemporaryDirectory();
    auto tmpFile = tempDir.createFile("testFile.tmp");
    REQUIRE(tmpFile);
    REQUIRE(tmpFile->exists());
    REQUIRE(tmpFile->filename() == "testFile.tmp");
    auto tmpFile2 = tempDir.createFile("testFile.tmp");
    REQUIRE(tmpFile2);
    REQUIRE(tmpFile2->exists());
    REQUIRE(tmpFile2->filename() == "testFile(1).tmp");
    auto tmpFile3 = tempDir.createFile("testFile");
    REQUIRE(tmpFile3);
    REQUIRE(tmpFile3->exists());
    REQUIRE(tmpFile3->filename() == "testFile");
    auto tmpFile4 = tempDir.createFile(".testFile");
    REQUIRE(tmpFile4);
    REQUIRE(tmpFile4->exists());
    REQUIRE(tmpFile4->filename() == ".testFile");
  }
}
