#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "CeresFile/File.h"
#include "CeresFile/TemporaryDirectory.h"
#include "Util.h"

TEST_CASE("Simple file wrapper behaves correctly!", "[file]") {
  {
    auto tempDir = TemporaryDirectory();
    auto filePath = tempDir.createTempFile().getPath();
    auto fileWrapper = File(filePath);
    REQUIRE(fileWrapper.exists());
    fileWrapper.deleteFile();
    REQUIRE(!fileWrapper.exists());
    REQUIRE(fileWrapper.create());
    REQUIRE(fileWrapper.exists());
    REQUIRE(!fileWrapper.create());

    REQUIRE(fileWrapper.getPath() == filePath);
    std::string dataStr = "testMessage \n testMessage2";
    auto data = stringToVector(dataStr);
    REQUIRE(fileWrapper.write(data, 0));
    {
      auto readData = fileWrapper.read();
      REQUIRE(readData);
      REQUIRE(vectorToString(*readData) == dataStr);
    }

    REQUIRE(fileWrapper.append(data));
    {
      auto readData = fileWrapper.read();
      REQUIRE(readData);
      REQUIRE(vectorToString(*readData) == dataStr + dataStr);
    }
    REQUIRE(fileWrapper.overwrite(data));
    {
      auto readData = fileWrapper.read();
      REQUIRE(readData);
      REQUIRE(vectorToString(*readData) == dataStr);
    }
    REQUIRE(fileWrapper.create(true));
    {
      auto readData = fileWrapper.read();
      REQUIRE(readData);
      REQUIRE(vectorToString(*readData) == "");
    }
  }
}