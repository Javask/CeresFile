#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "CeresFile/SimpleFile.h"
#include "CeresFile/TemporaryDirectory.h"

std::vector<char> stringToVector(const std::string& message) {
  auto data = std::vector<char>(message.size());
  for (size_t i = 0; i < data.size(); i++) {
    data[i] = message.c_str()[i];
  }
  return data;
}

std::string vectorToString(const std::vector<char>& message) {
  return std::string(message.data(), message.size());
}

TEST_CASE("Simple file wrapper behaves correctly!", "[file]") {
  {
    auto tempDir = TemporaryDirectory();
    auto filePath = tempDir.createTempFile();
    auto fileWrapper = SimpleFile(filePath);
    auto filePathStr = filePath.generic_string();
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