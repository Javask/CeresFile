#include <catch2/catch.hpp>
#include "CeresFile/BufferedFile.h"
#include "CeresFile/TemporaryDirectory.h"
#include "Util.h"
namespace CeresFile {
TEST_CASE("Buffered File behaves correctly", "[file]") {
  {
    auto tempDir = TemporaryDirectory();
    auto filePath = tempDir.createTempFile().getPath();
    auto fileWrapper = BufferedFile(filePath, false);
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
}  // namespace CeresFile